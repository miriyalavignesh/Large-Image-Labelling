import sys
def writeDict(dict, filename, sep):
    with open(filename, "w") as f:
        for i in sorted(dict.keys()):
        	f.write(str(i) + " " + sep.join([str(x) for x in dict[i]]) + "\n")

def insertNode(adjacency_list,prev,curr):
	if prev not in adjacency_list:
		adjacency_list[prev] = []
	adjacency_list[prev].append(curr)
	if curr not in adjacency_list:
		adjacency_list[curr] = []
	adjacency_list[curr].append(prev)
		
array = []
adjacency_list = {}
with open(sys.argv[1]) as f:
	for line in f:
		line = line.rstrip('\n')
		values = line.split(',')
		prev = int(values[0])
		for i in range(1,len(values)):
			curr = int(values[i])
			if prev != curr :
				insertNode(adjacency_list,prev,curr)
				prev = curr
		array.append(values)
for i in range(0,len(array[0])-1):
	prev = int(array[0][i])
	for j in range(1,len(array)-1):
		curr = int(array[j][i])
		if prev != curr:
			insertNode(adjacency_list,prev,curr)
			prev = curr
for i in adjacency_list.keys():
	adjacency_list[i] = set(adjacency_list[i])
	adjacency_list[i] = sorted(adjacency_list[i])
writeDict(adjacency_list,"neighbour.txt",",")
