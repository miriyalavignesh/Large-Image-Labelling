#Author Vignesh Miriyala
#run as python classifier.py test_data number_of_classes
import graphlab as graphlab
import sys
classes = int(sys.argv[2]) # no. of clasess
data = []
for i in range(1,classes+1):
	data.append( graphlab.SFrame.read_csv('train_class_'+str(i), delimiter='   ', header=False))
feature_label = []

#59 is the number of features in your training file without the label
for i in range(1,60):
	feature_label.append("X"+str(i))

model = []
for i in range(0,classes):
	model.append(graphlab.svm.create(data[i], target='X60', features=feature_label))
	#print "coefficients::"+ str(model[i-1]['coefficients'])
#save the model and re-use

test_data = graphlab.SFrame.read_csv(sys.argv[1], delimiter='   ', header=False);

predictions = []
for i in range(0,classes):
	predictions.append(model[i].predict(test_data, output_type='margin'))


f = open('vibhav_label', 'w')
results = []
for i in range(0,len(predictions[0])):
	max_index = 0
	for j in range(1,classes):
		if(predictions[max_index][i] < predictions[j][i]):
			max_index = j
	results.append(max_index+1)
	print>>f, max_index+1
	test_data["X61"] = max_index+1
print "results::"+str(results)
f.close()	
