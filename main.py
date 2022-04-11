def kmeans(k, max_iter=200, input, output):
    inputFile = open(input) #get vectors from file
    lines = inputFile.readlines()
    inputFile.close()
    vectors = []
    for line in lines:
        vectors.append(line.split(","))
    n = len(vectors) #update n and d
    d = len(vectors[0])
    e=0.001
    iterationNumber = 0
    if k<=1 or k>=n or max_iter<1: #check input
        print("Invalid Input!")
        return
    centroids = [vectors[i] for i in range(k)]
    while True:
        clusters = [[] for i in range(k)]
        for x in vectors: #assign vectors to clusters
            j = argmin(x, centroids)
            clusters[j].append(x)
        norms = [0 for i in range(k)]
        for i in range(k): #update centroids
            newCentroid = updateU(clusters[i])
            norms[i] = (oclidDistance(newCentroid, centroids[i]))**0.5
            centroids[i] = newCentroid
        iterationNumber+=1
        if max(norms)<e or iterationNumber == max_iter: #check convergence
            break
    outputFile = open(output, "w")
    for centroid in centroids:
        outputFile.write(','.join(centroid)+"/n")
    outputFile.close
    return


def oclidDistance(x1, x2):
    distance = 0
    for i in range(len(x1)):
        distance+= (x1[i]-x2[i])**2
    return distance


def argmin(x, centroids):
    index = 0
    minDistance = oclidDistance(x, centroids[0])
    for j in range(1, len(centroids)):
        distance = oclidDistance(x, centroids[j])
        if distance < minDistance:
            minDistance = distance
            index = j
    return index


def updateU(cluster):
    d = len(cluster[0])
    newU = [0 for i in range(d)]
    for vector in cluster:
        for i in range(d):
            newU[i]+=vector[i]
    for i in newU:
        newU[i] = newU[i]/(len(cluster))
    return newU
