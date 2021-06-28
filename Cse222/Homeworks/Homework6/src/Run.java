import java.io.*;
import java.util.LinkedList;
import java.util.Scanner;

/**
 * Driver class for E-Shopping Application
 */
public class Run {
    // App
    private static Application myApp;
    // [General Info]
    private static final File   csvFile     = new File("e-commerce-samples.csv");
    private static final File   productFile = new File("products.txt");
    private static final File   usersFile   = new File("users.txt");
    private static final File   orderFile   = new File("orders.txt");
    // Scanner for user input
    private static Scanner scanner = new Scanner(System.in);

    /**
     * Driver method
     * @param args not used
     * @throws IOException When file doesn't open or exist.
     */
    public static void main(String[] args) throws IOException {
        myApp = new Application(productFile, usersFile);

        myApp.csv_to_txt(csvFile);
        // Line breaks
        String lineBreak = "*********************************************";
        // object
        Application myApp = new Application(productFile, usersFile);
        // csv to txt
        myApp.csv_to_txt(csvFile);
        // users
        Customer c1 = new Customer(1234567, "asdFQW", productFile, orderFile);
        Trader t1 = new Trader  (12345678, "asd0FQW", "Shopmania", productFile, orderFile);

        // Invalid login
        System.out.println("\n" + lineBreak + " Unsuccessful login " + lineBreak + "\n");
        myApp.login(c1.getID(), c1.getPassword());
        myApp.login(t1.getID(), t1.getPassword());

        // registering - with invalid ID and password
        System.out.println("\n" + lineBreak + " Registering the users (Invalid ID and password size) " + lineBreak + "\n");
        myApp.register(c1);
        myApp.register(t1);

        c1 = new Customer(12345678, "qwerty", productFile, orderFile);
        t1 = new Trader  (98765432, "1234as", "Shopmania", productFile, orderFile);

        // registering - with valid ID and password
        System.out.println("\n" + lineBreak + " Registering the users (Valid ID and password) " + lineBreak + "\n");
        myApp.register(c1);
        myApp.register(t1);

        // login
        System.out.println("\n" + lineBreak + " Successful login " + lineBreak + "\n");
        myApp.login(c1.getID(), c1.getPassword());
        myApp.login(t1.getID(), t1.getPassword());


        // Customer - getting products by name
        LinkedList<Product> products = c1.getProductsByName("Shopmania Music Band A5");


        // printing
        System.out.println("\n" + lineBreak + " Searching for \"Shopmania Music Band A5\" (size = 2) " + lineBreak + "\n");
        for(int i=0; i<products.size(); i++)
            System.out.println(products.get(i));


        // Trader - removing one of the showed item
        System.out.println("\n" + lineBreak + " Removing an item(contains \"Shopmania Music Band A5\") " + lineBreak + "\n");
        t1.removeProduct("DIAEJ6Z8KQFWDEGH");


        // Customer - getting products by name
        products = c1.getProductsByName("Shopmania Music Band A5");


        // printing
        System.out.println("\n" + lineBreak + " Searching for \"Shopmania Music Band A5\" (size = 1) " + lineBreak + "\n");
        for(int i=0; i<products.size(); i++)
            System.out.println(products.get(i));


        // Trader - removing one of the showed item
        System.out.println("\n" + lineBreak + " Removing an item(last \"Shopmania Music Band A5\") " + lineBreak + "\n");
        t1.removeProduct("DIAEJ6Z8BEDTGGYY");


        // Customer - getting products by name
        products = c1.getProductsByName("Shopmania Music Band A5");


        // printing
        System.out.println("\n" + lineBreak + " Searching for \"Shopmania Music Band A5\" (size = 0) " + lineBreak + "\n");
        if(products.size() == 0)
            System.out.println("No item found with name \"Shopmania Music Band A5\"");


        // Adding an item with name "Shopmania Music Band A5"
        String ID = "1234ASDF1234QWER";
        String productName = "Shopmania Music Band A5(new added)";
        String productCategory = "Pens & Stationery >> Diaries & Notebooks >> Notebooks >> Designer";
        String description = "Specifications of Shopmania Music Band A5 Notebook Spiral Bound (Multicolor)";
        String trader = "Shopmania";
        int price = 999;
        int discountedPrice = 799;
        System.out.println("\n" + lineBreak + " Adding an item(contains \"Shopmania Music Band A5\") " + lineBreak + "\n");
        t1.addProduct(new Product(ID, productName, productCategory, description, trader, price, discountedPrice));



        // Customer - getting products by name
        products = c1.getProductsByName("Shopmania Music Band A5");


        // printing
        System.out.println("\n" + lineBreak + " Searching for \"Shopmania Music Band A5\" (size = 1) " + lineBreak + "\n");
        for(int i=0; i<products.size(); i++)
            System.out.println(products.get(i));


        // editing
        System.out.println("\n" + lineBreak + " Editing the recently added item " + lineBreak + "\n");
        t1.editProduct(ID, new Product(ID, productName, productCategory, description, trader, price+100, discountedPrice-200));


        // printing
        System.out.println("\n" + lineBreak + " Searching for \"Shopmania Music Band A5\" (size = 1) " + lineBreak + "\n");
        // Customer - getting products by name
        products = c1.getProductsByName("Shopmania Music Band A5");
        for(int i=0; i<products.size(); i++)
            System.out.println(products.get(i));


        // giving order
        System.out.println("\n" + lineBreak + " Giving an order - Customer " + lineBreak + "\n");
        c1.giveOrder(ID);
        // handling the order
        System.out.println("\n" + lineBreak + " Accepting the order - Trader " + lineBreak + "\n");
        t1.handleOrder(ID, false);



        // sorting products - all by same trader
        System.out.println("\n" + lineBreak + " Getting all products of trader: \"JJC\" " + lineBreak + "\n");
        products = c1.getProductsByTrader("JJC");


        // sorting by name and printing it
        System.out.println("\n" + lineBreak + " Sorting products by their name " + lineBreak + "\n");
        c1.sortByName(products);
        for(int i=0; i<products.size(); i++)
            System.out.println((i+1) + ") Product Name: " + products.get(i).getProductName() + "\t| Price: " + products.get(i).getPrice() + "\t| Discount Percentage: " + products.get(i).getDiscountPercentage());


        // sorting by price and printing it
        System.out.println("\n" + lineBreak + " Sorting products by their price " + lineBreak + "\n");
        c1.sortByPrice(products);
        for(int i=0; i<products.size(); i++)
            System.out.println((i+1) + ") Product Name: " + products.get(i).getProductName() + "\t| Price: " + products.get(i).getPrice() + "\t| Discount Percentage: " + products.get(i).getDiscountPercentage());


        // sorting by discount percentage and printing it
        System.out.println("\n" + lineBreak + " Sorting products by their discount percentage " + lineBreak + "\n");
        c1.sortByDiscountPercentage(products);
        for(int i=0; i<products.size(); i++)
            System.out.println((i+1) + ") Product Name: " + products.get(i).getProductName() + "\t| Price: " + products.get(i).getPrice() + "\t| Discount Percentage: " + products.get(i).getDiscountPercentage());

        // price filtering
        System.out.println("\n" + lineBreak + " Filtering products with prices between 1100 - 1200 " + lineBreak + "\n");
        c1.priceFilter(1100, 1200, products);
        for(int i=0; i<products.size(); i++)
            System.out.println((i+1) + ") Product Name: " + products.get(i).getProductName() + "\t| Price: " + products.get(i).getPrice() + "\t| Discount Percentage: " + products.get(i).getDiscountPercentage());

        // getting products
        System.out.println("\n" + lineBreak + " Getting all products of trader : Bosch " + lineBreak + "\n");
        products = c1.getProductsByTrader("Bosch");
        for(int i=0; i<products.size(); i++)
            System.out.println(products.get(i));

        // category filtering
        System.out.println("\n" + lineBreak + " Category limits : Automotive -> Vehicle Horns " + lineBreak + "\n");
        c1.categoryFilter("Vehicle Horns", "Automotive", products);
        for(int i=0; i<products.size(); i++)
            System.out.println(products.get(i));


        // [Driver code finished]
        System.out.println("\n" + lineBreak + " DRIVER CODE IS OVER & INTERACTIVE MENU STARTS " + lineBreak + "\n");

        // MENU
        menu();
    }

    private static void menu() {
        String userInput = "", userType = "";

        while(!userInput.equals("3")) {
            System.out.println("*** MENU ***");
            System.out.println("1) Login");
            System.out.println("2) Register");
            System.out.println("3) Exit");
            System.out.print  ("Enter: ");

            userInput = scanner.nextLine();

            if(!userInput.equals("3")) {
                System.out.println("***********");
                System.out.println("1) Customer");
                System.out.println("2) Trader");
                System.out.print  ("Enter: ");

                userType = scanner.nextLine();
            }

            switch(userInput) {
                case "1":
                    login(userType);
                    break;
                case "2":
                    register(userType);
                    break;
                case "3":
                    System.out.println("Good Bye!");
                    break;
                default:
                    System.out.println("ERROR! Invalid input.");
                    break;
            }
        }
    }

    private static void login(String userType) {
        // initializing scanner
        scanner = new Scanner(System.in);
        // variables
        int ID;
        String password, name = "empty";

        System.out.print("ID: ");
        ID = Integer.parseInt(scanner.nextLine());

        if(userType.equals("2")) {
            System.out.print("Name: ");
            name = scanner.nextLine();
        }

        System.out.print("Password: ");
        password = scanner.nextLine();

        if(myApp.login(ID, password)) {
            if(userType.equals("1"))
                userMenu(new Customer(ID, password, productFile, orderFile));
            else if(userType.equals("2"))
                userMenu(new Trader(ID, password, name, productFile, orderFile));
        }
    }

    private static void register(String userType) {
        // initializing scanner
        scanner = new Scanner(System.in);
        // variables
        int ID;
        String password, name = "empty";

        System.out.print("ID: ");
        ID = Integer.parseInt(scanner.nextLine());

        if(userType.equals("2")) {
            System.out.print("Name: ");
            name = scanner.nextLine();
        }

        System.out.print("Password: ");
        password = scanner.nextLine();

        try {
            if(userType.equals("1"))
                myApp.register(new Customer(ID, password, productFile, orderFile));
            else if(userType.equals("2"))
                myApp.register(new Trader(ID, password, name, productFile, orderFile));
        } catch (IOException exception) {
            System.out.println("\"ERROR! users.txt file couldn't open.\"");
        }

    }

    private static void userMenu(User user) {
        user.printCommandList();
    }

}
