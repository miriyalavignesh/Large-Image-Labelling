import graphlab as graphlab
import sys
data = []
for i in range(1,4):
	data.append( graphlab.SFrame.read_csv('train_class_'+str(i), delimiter='   ', header=False))
feature_label = []

#59 is the number of features in your training file without the label
for i in range(1,60):
	feature_label.append("X"+str(i))

model = []
for i in range(1,4):
	model.append(graphlab.svm.create(data[i-1], target='X60', features=feature_label))
	#print "coefficients::"+ str(model[i-1]['coefficients'])
#save the model and re-use

test_data = graphlab.SFrame.read_csv(sys.argv[1], delimiter='   ', header=False);

predictions = []
for i in range(1,4):
	predictions.append(model[i-1].predict(test_data, output_type='margin'))
	print "predictions::"+str(predictions[i-1])

results = []
for i in range(1,len(predictions[0])):
	max_index = 0
	for j in range(1,3):
		if(predictions[max_index][i-1] < predictions[j][i-1]):
			max_index = j
	results.append(max_index+1)
	test_data["X61"] = max_index+1
test_data.save('test_data_sframe');
print "results::"+str(results)
		
	
	
