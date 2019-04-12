/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Modified by Shimin Chen to demonstrate functionality for Homework 2
// April-May 2015

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.StringTokenizer;
import java.util.ArrayList;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapred.TextInputFormat;
import org.apache.hadoop.mapred.TextOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;


public class Hw2Part1 {

  public static class TokenizerMapper
      extends Mapper<Object, Text, SrcDestWritable, StatisticsWritable>{

      private static ArrayList<String> tokenize(String str) {

        ArrayList<String> tokens = new ArrayList<String>();
        StringTokenizer st = new StringTokenizer(str);

        while (st.hasMoreTokens())
          tokens.add(st.nextToken());

        return tokens;
      }

      public void map(Object key, Text value, Context context)
        throws IOException, InterruptedException {

        // debug
        System.out.println("Mapper Input: Value = " + value.toString());

        ArrayList<String> tokens = tokenize(value.toString());

        double time;
        if (tokens.size() == 3) {
          // try to parse the double
          try {
            time = Double.parseDouble(tokens.get(2));
          }
          catch (Exception e) {
            return;
          }

          SrcDestWritable outputKey = new SrcDestWritable(tokens.get(0), tokens.get(1));
          StatisticsWritable outputValue = new StatisticsWritable(1, time);
          context.write(outputKey, outputValue);

          // debug
          System.out.println("Mapper Output: Key = " + outputKey.toString() +
              "Value = " + outputValue.toString());
        }
      }
  }

  public static class StatisticsCombiner
      extends Reducer<SrcDestWritable, StatisticsWritable, SrcDestWritable, StatisticsWritable> {

      public void reduce(SrcDestWritable key, Iterable<StatisticsWritable> values,
          Context context) throws IOException, InterruptedException {

        int count = 0;
        double sum = 0;
        for (StatisticsWritable val : values) {
          count += val.getCount();
          sum += val.getSum();
        }
        StatisticsWritable outputValue = new StatisticsWritable(count, sum);
        System.out.println("Combiner Output: Key = " + key.toString() +
            "Value = " + outputValue.toString());
        context.write(key, outputValue);
      }
  }

  public static class StatisticsReducer
      extends Reducer<SrcDestWritable, StatisticsWritable, Text, Text> {

      public void reduce(SrcDestWritable key, Iterable<StatisticsWritable> values,
          Context context) throws IOException, InterruptedException {

        int count = 0;
        double sum = 0;
        for (StatisticsWritable val : values) {
          count += val.getCount();
          sum += val.getSum();
        }
        Text outputKey = new Text(String.format("%s %s", key.getSrc(), key.getDest()));
        Text outputValue = new Text(String.format("%d %.3f", count, sum / count));
        System.out.println("Reducer Output: Key = " + outputKey.toString() +
            "Value = " + outputValue.toString());
        context.write(outputKey, outputValue);
      }
  }

  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration();
    String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
    if (otherArgs.length != 2) {
      System.err.println("Usage: summary <input_file> <output_directory>");
      System.exit(2);
    }

    Job job = Job.getInstance(conf, "log summary");

    job.setJarByClass(Hw2Part1.class);

    job.setMapperClass(TokenizerMapper.class);
    job.setCombinerClass(StatisticsCombiner.class);
    job.setReducerClass(StatisticsReducer.class);

    job.setMapOutputKeyClass(SrcDestWritable.class);
    job.setMapOutputValueClass(StatisticsWritable.class);

    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(Text.class);

    // add the input paths as given by command line
    FileInputFormat.addInputPath(job, new Path(otherArgs[0]));

    // add the output path as given by the command line
    FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));

    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }

  public static class StatisticsWritable implements Writable {
    private IntWritable count;
    private DoubleWritable sum;

    public StatisticsWritable() {
      count = new IntWritable(0);
      sum = new DoubleWritable(0);
    }

    public StatisticsWritable(int count, double sum) {
      this.count = new IntWritable(count);
      this.sum = new DoubleWritable(sum);
    }

    public int getCount() {
      return count.get();
    }

    public double getSum() {
      return sum.get();
    }

    public void setCount(int count) {
      this.count.set(count);
    }

    public void getSum(double sum) {
      this.sum.set(sum);
    }

    public void readFields(DataInput in) throws IOException {
      count.readFields(in);
      sum.readFields(in);
    }

    public void write(DataOutput out) throws IOException {
      count.write(out);
      sum.write(out);
    }

    @Override
    public String toString() {
      return String.format("<%d, %.3f>", getCount(), getSum());
    }
  }


  public static class SrcDestWritable implements WritableComparable<SrcDestWritable> {
    private Text src, dest;

    public SrcDestWritable() {
      src = new Text();
      dest = new Text();
    }

    public SrcDestWritable(String src, String dest) {
      this.src = new Text(src);
      this.dest = new Text(dest);
    }

    public String getSrc() {
      return src.toString();
    }

    public String getDest() {
      return dest.toString();
    }

    public void setSrc(String src) {
      this.src.set(src);
    }

    public void setDest(String dest) {
      this.dest.set(dest);
    }

    public void readFields(DataInput in) throws IOException {
      src.readFields(in);
      dest.readFields(in);
    }

    public void write(DataOutput out) throws IOException {
      src.write(out);
      dest.write(out);
    }

    public int compareTo(SrcDestWritable that) {
      return this.toString().compareTo(that.toString());
    }

    public int hashCode() {
      return this.toString().hashCode();
    }

    @Override
    public String toString() {
      return String.format("<%s, %s>", getSrc(), getDest());
    }
  }
}
