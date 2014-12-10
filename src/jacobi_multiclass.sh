rm -rf jacobi_test1/laplacian_output1
./jacobi --matrix=jacobi_test1/ --initial_vec=result_label1 --rows=$1 --cols=$1 --max_iter=25
mv x.out.1_of_1 jacobi_test1/laplacian_output1
rm -rf jacobi_test2/laplacian_output2
./jacobi --matrix=jacobi_test2/ --initial_vec=result_label2 --rows=$1 --cols=$1 --max_iter=25
mv x.out.1_of_1 jacobi_test2/laplacian_output2
rm -rf jacobi_test3/laplacian_output3
./jacobi --matrix=jacobi_test3/ --initial_vec=result_label3 --rows=$1 --cols=$1 --max_iter=25
mv x.out.1_of_1 jacobi_test3/laplacian_output3
rm -rf jacobi_test4/laplacian_output4
./jacobi --matrix=jacobi_test4/ --initial_vec=result_label4 --rows=$1 --cols=$1 --max_iter=25
mv x.out.1_of_1 jacobi_test4/laplacian_output4
rm -rf jacobi_test5/laplacian_output5
#./jacobi --matrix=jacobi_test5/ --initial_vec=result_label5 --rows=$1 --cols=$1 --max_iter=25
#mv x.out.1_of_1 jacobi_test5/laplacian_output5

