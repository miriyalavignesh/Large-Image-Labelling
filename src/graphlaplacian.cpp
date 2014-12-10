#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <time.h>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <graphlab.hpp>
#include <graphlab/graph/distributed_graph.hpp>
using namespace graphlab;

float gaussian_kernel_scale_parameter = 2;
float lambda = 0.05;
float alpha = 0.52;
//data point
struct vertex_data {
	std::vector<float> x;
	float D_ii;
	vertex_data():x(), D_ii(0.0) {};
	explicit vertex_data(const std::vector<float>& x_in) :
		x(x_in), D_ii(0.0) {}
	void save(graphlab::oarchive& oarc) const {
		oarc << x.size();
		for(size_t i=0;i<x.size();++i)
			oarc << x[i];
		oarc << D_ii;
	}
	void load(graphlab::iarchive& iarc) {
		size_t size = 0;
		iarc >> size;
		for(size_t i=0;i<size;++i){
			float temp = 0.0;
			iarc >> temp;
			x.push_back(temp);
		}
		iarc >> D_ii;
	}
};

struct edge_data{
	float A_ij;
	bool nearest;
	edge_data() : A_ij(0.0), nearest(false){}
	void save(graphlab::oarchive& oarc) const {
		oarc << A_ij << nearest;
	}
	void load(graphlab::iarchive& iarc) {
		iarc >> A_ij >> nearest;
	}
};

typedef graphlab::distributed_graph<vertex_data, edge_data> graph_type;

bool line_parser(graph_type& graph, const std::string& filename, const std::string& line) {
	if (line.empty()) return true;
	size_t id = 0;
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;
	namespace phoenix = boost::phoenix;
	vertex_data vtx;
	const bool success = qi::phrase_parse
		(line.begin(), line.end(),
		 // Begin grammar
		 (
		  qi::ulong_[phoenix::ref(id) = qi::_1] >> -qi::char_(",") >>
		  (qi::double_[phoenix::push_back(phoenix::ref(vtx.x), qi::_1)] % -qi::char_(",") )
		 )
		 ,
		 // End grammar
		 ascii::space);
	if (!success) return false;
	graph.add_vertex(id, vtx);
	//	for(size_t i=1;i<id;++i){
	//		graph.add_edge(i, id);
	//	}
	return true;
}

bool neighbour_parser(graph_type& graph, const std::string& filename, const std::string& line) {
	if (line.empty()) return true;
	size_t id = 0;
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;
	namespace phoenix = boost::phoenix;
	vertex_data vtx;
	const bool success = qi::phrase_parse
		(line.begin(), line.end(),
		 // Begin grammar
		 (
		  qi::ulong_[phoenix::ref(id) = qi::_1] >> -qi::char_(",") >>
		  (qi::double_[phoenix::push_back(phoenix::ref(vtx.x), qi::_1)] % -qi::char_(",") )
		 )
		 ,
		 // End grammar
		 ascii::space);
	if (!success) return false;
	//	graph.add_vertex(id, vtx);
	for(size_t i=0;i<vtx.x.size();++i)
	{         graph.add_edge(id,vtx.x[i]);
		//std::cout<<id<<vtx.x[i];
	}
	return true;
}

float similarity(const std::vector<float>& v1, const std::vector<float>& v2) {
	float ret = 0.0;//,temp=0.0;
	//std::cout<<" "<<v1[0]<<" "<<v2[0]<<"\n";
	for (size_t i = 0; i < v1.size(); ++i) {
		float tmp = v1[i] - v2[i];
		ret += tmp * tmp;
	}
//	ret=sqrt(ret);
	//  temp=exp(-ret / gaussian_kernel_scale_parameter);
	//std::cout << "Temp= "<<temp<<"\n";
	return exp(-ret / gaussian_kernel_scale_parameter);
}

void calc_similarities(graph_type::edge_type& edata) {
	edata.data().A_ij =  similarity(edata.source().data().x, edata.target().data().x);
	//std::ofstream affinity;
	//affinity.open("affinity_matrix.txt");
	//std::cout<< edata.data().A_ij << "\n";
	//affinity.close();
}

//compute sums over rows and then take inverse square root
class calc_degrees: public graphlab::ivertex_program<graph_type,
	float>, public graphlab::IS_POD_TYPE {
		public:
			//gather A_ij
			edge_dir_type gather_edges(icontext_type& context,
					const vertex_type& vertex) const {
				return graphlab::OUT_EDGES;
			}
			float gather(icontext_type& context, const vertex_type& vertex,
					edge_type& edge) const {
				return edge.data().A_ij;
			}
			//assign a cluster, considering the clusters of neighbors
			void apply(icontext_type& context, vertex_type& vertex,
					const gather_type& total) {
				vertex.data().D_ii = 1.0 / sqrt(1+total);
				//std::cout<<"vertexD_ii. "<<vertex.data().D_ii<<"\n";
			}
			edge_dir_type scatter_edges(icontext_type& context,
					const vertex_type& vertex) const {
				return graphlab::NO_EDGES;
			}
			void scatter(icontext_type& context, const vertex_type& vertex,
					edge_type& edge) const {
			}
	};

void mult_D(graph_type::edge_type& edata) {
	edata.data().A_ij = edata.data().A_ij * edata.source().data().D_ii * edata.target().data().D_ii;
	//std::cout<<"edge "<<edata.data().A_ij<<"\n";
}

class graph_writer {

	public:
		std::string save_vertex(graph_type::vertex_type v) {
			std::stringstream strm;
			size_t vid = v.id();
			if(vid == 0)
				return "";
			strm << vid << " " << vid << " 1.0\n";
			return strm.str();
		}
};

class affinity_writer {
	public:
		std::string save_vertex(graph_type::vertex_type v) {
			std::stringstream strm;
			return "";
		}


	public:
		std::string save_edge(graph_type::edge_type e) {
			const float& A_ij = e.data().A_ij;

			std::stringstream strm;

			strm << e.source().id() << " " << e.target().id() << " " <<
				A_ij << "\n";

			return strm.str();
		}
};


class neighbour_writer {

	public:
		std::string save_vertex(graph_type::vertex_type v) {
			std::stringstream strm;
			size_t vid = v.id();
			if(vid == 0)
				return "";
			strm << v.id() << " " << v.id() << " " <<
				1.0 - (1.0-alpha)*(v.data().D_ii * v.data().D_ii) << "\n";
			return strm.str();
		}
		std::string save_edge(graph_type::edge_type e) {
			const float& A_ij = e.data().A_ij;

			std::stringstream strm;
			if(A_ij > 0.0){
				strm << e.source().id() << " " << e.target().id() << " " <<
					-((1.0 - alpha)*A_ij) << "\n";
			}
			return strm.str();
		}
};
int main(int argc, char** argv) {
	std::cout << "construct graph Laplacian for solving Linear System of Equations.\n\n";
	std::string datafile;
	std::string neighbour;
	graphlab::command_line_options clopts
		("Constructing graph Laplacian for spectral clustering");
	clopts.attach_option("data", datafile,
			"Input file. Each line hold a sample id followed by a white-space or "
			"comma separated numeric vector. Id should start from 1");
	clopts.attach_option("neighbour", neighbour,
			"Input file. Each line hold a sample id followed by a white-space or "
			"comma separated numeric vector. Id should start from 1");
	clopts.attach_option("sigma", gaussian_kernel_scale_parameter,
			"Scale parameter for Gaussian kernel.");
	clopts.attach_option("alpha", alpha,
			"alpha parameter for laplacian");
	if(!clopts.parse(argc, argv)) return EXIT_FAILURE;
	if (datafile == "") {
		std::cout << "--data is not optional\n";
		return EXIT_FAILURE;
	}
	gaussian_kernel_scale_parameter *= 2.0*gaussian_kernel_scale_parameter;
	//construct graph
	graphlab::mpi_tools::init(argc, argv);
	graphlab::distributed_control dc;
	graph_type graph(dc, clopts);
	graph.load(datafile,
			line_parser);
	graph.load(neighbour,
			neighbour_parser);
	graph.finalize();
	time_t start, end;
	time(&start);
	//calculate similarities
	graph.transform_edges(calc_similarities);
	const std::string output =  "glap";


	graphlab::omni_engine<calc_degrees> engine(dc, graph, "sync", clopts);
	engine.signal_all();
	engine.start();
	//multiply D^-1/2 with affinity matrix with D^-1/2
	graph.transform_edges(mult_D);
	/**graph.save(
	  output + "_affinity",
	  affinity_writer(), false, //set to true if each output file is to be gzipped
	  true, //whether vertices are saved
	  true,1); //whether edges are saved**/
	time(&end);
	dc.cout() << "graph calculation time is " << (end - start) << " sec\n";
	dc.cout() << "writing...for alpha"<<alpha<<"\n";
	//graph.save(
	//		outputname + "_diag",
	//		graph_writer(), false, //set to true if each output file is to be gzipped
	//		true, //whether vertices are saved
	//		false,1); //whether edges are saved
	graph.save("A",
			neighbour_writer(), false, //set to true if each output file is to be gzipped
			true, //whether vertices are saved
			true,1); //whether edges are saved
	//write the number of data
	graphlab::mpi_tools::finalize();
	return EXIT_SUCCESS;
}
