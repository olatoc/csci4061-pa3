# CSCI4061-Assignment3

                /*
                * test machine: csel-kh1250-01.cselabs.umn.edu * date: 11/13/19
                * name: Oliver Latocki, Jonathan Sulisz
                * x500: latoc004, sulis005  
                */

• **The purpose of our program**

In this programming assignment, we want to work on both sides in problem “word count statistics”, which is the multithreaded version of MapReduce application in programming assignment 2.

This program will count the histogram of the starting letter for each world from a given text file. Multithreading will be utilized to create a producer to read the file and multiple consumers to process the smaller piece data. We will have two forms of synchronization: a shared queue synchronized between producer and consumers, and a global result histogram synchronized by consumers. 

# • How to compile the program


Using the Makefile provided in the pa3 folder, run:  

`make`  

***ASSUMPTIONS***

-Program is executed in this way:

`./wcs 100 testing/test4.txt -p`  

Where the complete relative filepath is given for the input file.  

# • What exactly your program does

***Producer***  

*void init_producer(char \*filename){*

Initializes producer by creating a p_args_t object and copying the filename to it. Finishing the function by creating a thread of the ID.

*void \*produce(void \*filename){*


Initialize producer functionality: logging the Producer's intialization, calling read_file()  


*void read_file(char \*filename){*


Read the file specified by the filepath "filename" line-by-line. Insert lines from the file into the shared queue in a thread-safe way, using mutex locks and a contition variable. Notify the consumer threads when the EOF is reached using a global. Log which line is being read if specified.


***Consumer***  

*void init_consumers(int consumer_count){*


Create consumer threads specified as by a command line argument. Give each thread an ID ranging from 0 to consumer# - 1.


*void \*consume(void \*arg_ptr){*

Extracts lines line-by-line from shared queue, performing word statistics on each line (counts number of occurances of words which start with each letter of the alphabet). Each thread terminates upon the producer's EOF signal and the queue's being empty. mutex locks and a condition variable are used throught to ensure thread-safe functionality.  

*void stats(char \*line, int id){*


Counts number of occurances of words which start with each letter of the alphabet. Done for each line read by each consumer.


# **• Your and your partner's individual contributions**  

Oliver Latocki: wrote most of final code

Jonathan: wrote code alternative to the final code. 

# • If you have attempted extra credit  

No

