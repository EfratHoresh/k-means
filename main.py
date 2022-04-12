def kmeans(k, input, max_iter=200):
    input_file = open(input)  # get vectors from file
    lines = input_file.readlines()
    input_file.close()
    vectors = []
    for line in lines:
        tmp_vector = line.split(",")
        vectors.append([float(i) for i in tmp_vector])
    n = len(vectors)  # update n and d
    d = len(vectors[0])
    e = 0.001
    iteration_number = 0
    if k <= 1 or k >= n or max_iter < 1:  # check input
        print("Invalid Input!")
        return
    centroids = [vectors[i] for i in range(k)]
    while True:
        clusters = [[] for i in range(k)]
        for x in vectors:  # assign vectors to clusters
            j = argmin(x, centroids)
            clusters[j].append(x)
        norms = [0 for i in range(k)]
        for i in range(k):  # update centroids
            new_centroid = updateU(clusters[i])
            norms[i] = (oclidDistance(new_centroid, centroids[i]))**0.5
            centroids[i] = new_centroid
        iteration_number += 1
        if max(norms) < e or iteration_number == max_iter:  # check convergence
            break
    return centroids


def oclidDistance(x1, x2):
    distance = 0
    for i in range(len(x1)):
        distance += (x1[i]-x2[i])**2
    return distance


def argmin(x, centroids):
    index = 0
    min_distance = oclidDistance(x, centroids[0])
    for j in range(1, len(centroids)):
        distance = oclidDistance(x, centroids[j])
        if distance < min_distance:
            min_distance = distance
            index = j
    return index


def updateU(cluster):
    d = len(cluster[0])
    new_u = [0 for i in range(d)]
    for vector in cluster:
        for i in range(d):
            new_u[i] += vector[i]
    for i in range(len(new_u)):
        new_u[i] = new_u[i]/(len(cluster))
    return new_u

args = input().split()
if len(args) == 4:
    if not args[0].isnumeric() or not args[1].isnumeric():
        print("Invalid Input!")
        quit()
    centroids = kmeans(int(args[0]), args[2], int(args[1]))
elif len(args) == 3:
    if not args[0].isnumeric():
        print("Invalid Input!")
        quit()
    centroids = kmeans(int(args[0]), args[1])
else:
    print("Invalid Input!")
    quit()
output_file = open(args[3], "w")
for centroid in centroids:
    centroid_string = ['{:.4f}'.format(x) for x in centroid]
    output_file.write(','.join(centroid_string)+"\n")
output_file.close


# if __name__ == "main":
#     main()
