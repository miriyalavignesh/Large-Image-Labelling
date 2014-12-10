# Naive Script to help myself for running different values of alpha and combining results of modules
#!/bin/bash
# $1 - alpha $2 - features $3 - train_folder $4 - labelfile
echo "running for alpha".$1;
python classifier.py $2 $3 4 $1
echo "svm labelling finished-------"
cp result_label ~/Documents/pds-project/opencv_tinkering/
./addlines.sh $2 > data.txt
echo "creating neighbours data"
python adjacent_superpixel.py $4
NUM_FEATURES=`sed -n '$=' data.txt`
cp neighbour.txt data.txt ~/GraphLab/graphlab/debug/apps/graphlaplacian
~/GraphLab/graphlab/debug/apps/graphlaplacian/graphlaplacian --data=data.txt --neighbour=neighbour.txt --alpha=$1
cp A.1_of_1 ~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_test1/A
cp A.1_of_1 ~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_test2/A
cp A.1_of_1 ~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_test3/A
cp A.1_of_1 ~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_test4/A
#cp A.1_of_1 ~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_test5/A
cp result_label1  ~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_test1/
cp result_label2  ~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_test2/
cp result_label3  ~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_test3/
cp result_label4  ~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_test4/
#cp result_label5  ~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_test5/
cd ~/GraphLab/graphlab/debug/toolkits/linear_solvers
~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_multiclass.sh $NUM_FEATURES
cd -
cp ~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_test1/laplacian_output1 ~/Documents/pds-project/jacobi_comparision/
cp ~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_test2/laplacian_output2 ~/Documents/pds-project/jacobi_comparision/
cp ~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_test3/laplacian_output3 ~/Documents/pds-project/jacobi_comparision/
cp ~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_test4/laplacian_output4 ~/Documents/pds-project/jacobi_comparision/
#cp ~/GraphLab/graphlab/debug/toolkits/linear_solvers/jacobi_test5/laplacian_output5 ~/Documents/pds-project/jacobi_comparision/
cd ~/Documents/pds-project/jacobi_comparision
java ReadLine $NUM_FEATURES
cp output.txt ~/Documents/pds-project/opencv_tinkering/
cd -
cd ~/Documents/pds-project/opencv_tinkering/
rm -rf ~/Documents/pds-project/opencv_tinkering/res.jpg
python ~/Documents/pds-project/opencv_tinkering/image_gen.py ~/Documents/pds-project/$5 ~/Documents/pds-project/$4 output.txt 
cp ~/Documents/pds-project/opencv_tinkering/test.jpg ~/Documents/pds-project/$3"test-"$1".jpg"
cp ~/Documents/pds-project/opencv_tinkering/res.jpg ~/Documents/pds-project/$3"res-"$1".jpg"
rm -rf ~/Documents/pds-project/opencv_tinkering/res.jpg
python ~/Documents/pds-project/opencv_tinkering/image_gen.py ~/Documents/pds-project/$5 ~/Documents/pds-project/$4 result_label 
cp ~/Documents/pds-project/opencv_tinkering/test.jpg ~/Documents/pds-project/$3"svmtest-"$1".jpg"
cp ~/Documents/pds-project/opencv_tinkering/res.jpg ~/Documents/pds-project/$3"svmres-"$1".jpg"
cd -

