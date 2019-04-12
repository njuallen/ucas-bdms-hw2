all: Hw2Part1.jar

Hw2Part1.jar: Hw2Part1.java Hw2Part1-manifest.txt
	javac Hw2Part1.java
	jar cfm Hw2Part1.jar Hw2Part1-manifest.txt Hw2Part1*.class

run: Hw2Part1.jar
	hdfs dfs -copyFromLocal -f example-input.txt /hw2/example-input.txt
	hdfs dfs -rm -f -r /hw2/output
	hadoop jar ./Hw2Part1.jar /hw2/example-input.txt /hw2/output
	hdfs dfs -cat '/hw2/output/part-*'

test: Hw2Part1.jar
	hdfs dfs -copyFromLocal -f test-input.txt /hw2/test-input.txt
	hdfs dfs -rm -f -r /hw2/output
	hadoop jar ./Hw2Part1.jar /hw2/test-input.txt /hw2/output
	hdfs dfs -cat '/hw2/output/part-*'

input: Hw2Part1.jar
	hdfs dfs -copyFromLocal -f part1-input /hw2/
	hdfs dfs -rm -f -r /hw2/output
	hadoop jar ./Hw2Part1.jar /hw2/part1-input/input_0 /hw2/output
	hdfs dfs -cat '/hw2/output/part-*'
	hdfs dfs -rm -f -r /hw2/output
	hadoop jar ./Hw2Part1.jar /hw2/part1-input/input_1 /hw2/output
	hdfs dfs -cat '/hw2/output/part-*'
	hdfs dfs -rm -f -r /hw2/output
	hadoop jar ./Hw2Part1.jar /hw2/part1-input/input_2 /hw2/output
	hdfs dfs -cat '/hw2/output/part-*'
