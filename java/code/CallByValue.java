public class CallByValue {

    public static void main(String[] args) throws Exception {
        // checkString();
    }

    private static void checkPrimitiveType() {
        int a = 10;
        int b = a;

        System.out.println("a = " + System.identityHashCode(a)); // 1057941451
        System.out.println("b = " + System.identityHashCode(b)); // 1057941451

        b = 10;
        System.out.println("b = " + System.identityHashCode(b)); // 1057941451

        b = 11;
        System.out.println("b = " + System.identityHashCode(b)); // 434091818

        b = 10;
        System.out.println("b = " + System.identityHashCode(b)); // 1057941451

        int c = 10;
        System.out.println("c = " + System.identityHashCode(c)); // 1057941451
    }

    private static void checkObject() {
        User a = new User("apple");
        User b = a;

        System.out.println("a = " + a); // CallByValue$User@75bd9247
        System.out.println("b = " + b); // CallByValue$User@75bd9247
        System.out.println("a == b ? " + (a == b)); // true

        b.printName(); // apple
        b.name = "kiwi";
        b.printName(); // kiwi
        a.printName(); // kiwi

        System.out.println("a = " + a); // CallByValue$User@75bd9247
        System.out.println("b = " + b); // CallByValue$User@75bd9247
    }

    private static void checkObjectNew() {
        User a = new User("apple");
        User b = new User("apple");

        System.out.println("a = " + a); // CallByValue$User@3d24753a
        System.out.println("b = " + b); // CallByValue$User@59a6e353
        System.out.println("a == b ? " + (a == b)); // false
    }

    private static void checkObjectChange() {
        User a = new User("apple");
        a.printName(); // apple
        System.out.println("a = " + System.identityHashCode(a)); // 1975358023

        a.changeObject(a);
        a.printName(); // apple
        System.out.println("a = " + System.identityHashCode(a)); // 1975358023
    }

    private static void checkString() {
        String a = "apple"; // String Constant Pool 에 할당
        String b = "apple";
        System.out.println("a == b ? " + (a == b)); // true

        String c = new String("apple"); // Heap 에 할당
        System.out.println("a == c ? " + (a == c)); // false
        System.out.println("a.equal(c) ? " + a.equals(c)); // true
    }

    private static class User {
        String name;
        User(String name) {
            this.name = name;
        }

        void printName() {
            System.out.println(name);
        }

        void changeObject(User b) {
            b = new User("kiwi");
            b.printName(); // kiwi
            System.out.println("changeObject = " + System.identityHashCode(b)); // 398887205
        }
    }
}