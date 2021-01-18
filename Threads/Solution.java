import java.io.*;
import java.util.*;
import java.text.*;
import java.math.*;
import java.util.regex.*;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;


class Counter{

    int count = 0;
    long threadId = 999999;

    void  add(){

        if(threadId != Thread.currentThread().getId()){
            threadId = Thread.currentThread().getId();
        }
        else{
            System.out.println("Error : Same thread id cannot proceed");
            return;
        }

        for (int i = 0; i < 100000; i++) {
            count++;

        }
        try{
            Thread.sleep(1);
        }
        catch(Exception e){

        }
    }

    int getTotalCount(){
        return count;
    }
}


public class Solution {
    

    public static void main(String[] args) {
        int i = 0;
        int threadCount;
        Scanner scan = new Scanner(System.in);
        int integer = scan.nextInt();
        threadCount = integer / 100000;
        Counter counter = new Counter();

        if (threadCount <= 0)
            return;

        class ThreadExposeClass extends Thread {
            @Override
            public void run() {
                counter.add();
            }
        }

        ThreadExposeClass[] threadArray = new ThreadExposeClass[threadCount];

        for (i = 0; i < threadCount; i++)
            threadArray[i] = new ThreadExposeClass();

        for (i = 0; i < threadCount; i++) {
            threadArray[i].start();
            try {
                threadArray[i].join();
            } catch (InterruptedException e) {}
        }
        System.out.println(counter.getTotalCount());
    }

}
