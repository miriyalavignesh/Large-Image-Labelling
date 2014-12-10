import java.io.*;
import java.util.*;

class ReadLine{

	public static HashMap<Integer, Double> valuePairs(String filename){
		HashMap<Integer, Double> map = new HashMap<Integer, Double>();
		try {
			BufferedReader br = new BufferedReader(new FileReader(filename));
			String line;
		
			while((line = br.readLine()) != null){

				String part = line;
				String[] parts = part.split("\\s+");
	
				String part1 = parts[0];
				String part2 = parts[1];

				int key_num = Integer.parseInt(part1);
				double val_num = Double.parseDouble(part2);
				map.put(key_num, val_num);
			//	System.out.println(key_num+" "+val_num);
			}
		//	System.out.println("In function HashMap");
		}
		catch(Exception e){
			System.out.println(e);
		}

		return map;
	}

	public static void main(String[] args){
		
		Scanner input = new Scanner(System.in);
		
		HashMap<Integer, Double> map1 = new HashMap<Integer, Double>();
		HashMap<Integer, Double> map2 = new HashMap<Integer, Double>();
		HashMap<Integer, Double> map3 = new HashMap<Integer, Double>();
		HashMap<Integer, Double> map4 = new HashMap<Integer, Double>();
		HashMap<Integer, Double> map5 = new HashMap<Integer, Double>();

		ArrayList<String> list = new ArrayList<String>();
		//for(int i = 0; i < 5; i++){
		//	String filename = input.next();
		//	System.out.println
		//	list.add(filename);
		//}

/*		map1 = valuePairs(list.get(0));
		map2 = valuePairs(list.get(1));
		map3 = valuePairs(list.get(2));
		map4 = valuePairs(list.get(3));
		map5 = valuePairs(list.get(4));
*/
		
		map1 = valuePairs("laplacian_output1");
		map2 = valuePairs("laplacian_output2");
		map3 = valuePairs("laplacian_output3");
		map4 = valuePairs("laplacian_output4");
//		map5 = valuePairs("laplacian_output5");
	

		int size = Integer.parseInt(args[0]);
		int[] array = new int[size];
		for(int i = 0; i < size; i++){
			double max = 0; 
			int k = 0;

			if(map1.containsKey(i+1)){
				max = map1.get(i+1);
				k = 1;
			}

			if(map2.containsKey((i+1))){
				if(map2.get((i+1)) > max){
					max = map2.get(i + 1);
					k = 2;
				}
			}

			if(map3.containsKey(i+1)){
				if(map3.get(i+1) > max){
					max = map3.get(i + 1);
					k = 3;
				}
			}
	
			if(map4.containsKey(i+1)){
				if(map4.get(i+1) > max){
					max = map4.get(i + 1);
					k = 4;
				}
			}

			if(map5.containsKey(i+1)){
				if(map5.get(i+1) > max){
					max = map5.get(i + 1);
					k = 5;
				}
			}

			array[i] = k;
		}

		try {
		BufferedWriter outputWriter = new BufferedWriter(new FileWriter("output.txt"));

		for(int i = 0; i < size; i++){
			
			//outputWriter.write((i+1) + " ");
			outputWriter.write(""+array[i]);
			outputWriter.newLine();

		}

		outputWriter.flush();
		outputWriter.close();
		}
		catch(Exception e) {
		}
	}
}
