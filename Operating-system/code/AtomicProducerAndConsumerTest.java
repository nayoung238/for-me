package com.example;

import java.util.concurrent.atomic.AtomicBoolean;

public class AtomicProducerAndConsumerTest {

    static int count = 0;
    static int turn = 0;
    static AtomicBoolean[] flag = new AtomicBoolean[2];
    static {
        for(int i = 0; i < flag.length; i++)
            flag[i] = new AtomicBoolean();
    }

    public static void main(String[] args) {
        Producer producer = new Producer();
        Consumer consumer = new Consumer();

        producer.start();
        consumer.start();

        try {
            producer.join();
            consumer.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("count " + count);
    }

    static class Producer extends Thread {
        static int other = 1;
        @Override
        public void run() {
            for(int i = 0; i < 10000; i++) {
                flag[1- other].set(true);
                turn = other;
                while(flag[other].get() && turn == other);

                count++;
                flag[1 - other].set(false);
            }
        }
    }

    static class Consumer extends Thread {
        static int other = 0;
        @Override
        public void run() {
            for(int i = 0; i < 10000; i++) {
                flag[1 - other].set(true);
                turn = other;
                while(flag[other].get() && turn == other);

                count--;
                flag[1 - other].set(false);
            }
        }
    }
}
