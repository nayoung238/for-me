package com.example;

public class ProducerAndConsumer {

    static int count = 0;
    static int turn = 0;
    static boolean[] flag = new boolean[2];

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
                flag[1- other] = true;
                turn = other;
                while(flag[other] && turn == other);

                count++;
                flag[1 - other] = false;
            }
        }
    }

    static class Consumer extends Thread {
        static int other = 0;
        @Override
        public void run() {
            for(int i = 0; i < 10000; i++) {
                flag[1 - other] = true;
                turn = other;
                while(flag[other] && turn == other);

                count--;
                flag[1 - other] = false;
            }
        }
    }
}