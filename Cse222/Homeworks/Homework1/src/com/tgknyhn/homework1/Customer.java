package com.tgknyhn.homework1;

import java.util.Scanner;

/**
 * Implements User interface. It contains all necessary methods and fields for a customer to shop in a store. 
 */
public class Customer implements User {
    static private int customerNumber = 0; // shows how many subscribed customer are there in the furniture company
    final static private int MAX_ORDER = 5;
    private String name, surname;
    private String email;
    private String password;
    private String address;
    private long phoneNumber;
    private int customerID;
    private String[] previousOrders;
    private int orderCount;

    /**
     * Only constructor for Customer class. It takes name, surname, email and password as parameters
     * and initializes Customer class with these values.
     * @param name Name of the Customer
     * @param surname Surname of the Customer
     * @param email E-mail of the Customer
     * @param password Password of the Customer
     */
    public Customer(String name, String surname, String email, String password) {
        setName(name);
        setSurname(surname);
        setEmail(email);
        setPassword(password);
        setOrderCount(0);
        createID();
        previousOrders = new String[MAX_ORDER];
    }

    @Override
    public String getName() { return name; }

    @Override
    public String getSurname() { return surname; }

    @Override
    public String getEmail() { return email; }

    @Override
    public String getPassword() { return password; }

    /**
     * Returns the ID of customer
     * @return ID of the customer
     */
    public int getCustomerID() { return customerID; }

    /**
     * Returns address of the customer
     * @return Address of the customer
     */
    public String getAddress() { return address; }

    /**
     * Returns phone number of the customer
     * @return Phone number of the customer
     */
    public long getPhoneNumber() { return phoneNumber; }

    /**
     * Returns full name of the previous order
     * @param index Shows which previous order to return
     * @return Full name of the previous order
     */
    public String getOrder(int index) { return previousOrders[index]; }

    /**
     * Returns number of orders that customer made
     * @return Number of orders that customer made
     */
    public int getOrderCount() { return orderCount; }

    @Override
    public void setName(String name) { this.name = name; }

    @Override
    public void setSurname(String surname) { this.surname = surname; }

    @Override
    public void setEmail(String email) { this.email = email; }

    @Override
    public void setPassword(String password) { this.password = password; }

    /**
     * Sets customer's address as the given parameter.
     * @param address Address of the customer
     */
    public void setAddress(String address) { this.address = address; }

    /**
     * Sets customer's phone number as the given parameter.
     * @param phoneNumber Phone number of the customer.
     */
    public void setPhoneNumber(long phoneNumber) { this.phoneNumber = phoneNumber; }

    /**
     * Executed when customer buys a product. Sets bought item's information as previous order to reach them later.
     * Also increases the order count by one
     * @param productName Name of the product which bought by the customer
     * @param productModel Model of the product which bought by the customer
     * @param productColor Color of the product which bought by the customer
     * @throws ArrayIndexOutOfBoundsException After 5 order, customer reaches limit. Any execution of this function after 5 order will throw this exception.
     */
    public void setOrder(String productName, Model productModel, Color productColor) throws ArrayIndexOutOfBoundsException {
        // setting product's full name
        String product = productName + " - " + productModel + " - " + productColor;
        // set full name as previous order
        previousOrders[getOrderCount()] = product;
        // increasing order count by one
        setOrderCount(getOrderCount() + 1);
    }

    /**
     * Sets customer's order count as the given parameter
     * @param count Number of orders that customer made
     */
    public void setOrderCount(int count) { orderCount = count; }


    /**
     * Creates an ID for the customer
     */
    public void createID() {
        customerNumber++;

        customerID = customerNumber + 999;
    }



    @Override
    public void commands(Company company, FurnitureBranch[] branches, User[] employees) {
        // Scanner
        Scanner scanner = new Scanner(System.in);
        String input = "";
        // loop condition
        boolean goBack = false;

        while(!goBack) {
            System.out.println("1) Search for a product");
            System.out.println("2) List of products");
            System.out.println("3) Previous orders");
            System.out.println("4) Back");
            System.out.print("Enter: ");

            input = scanner.next();

            switch (input) {
                case "1":
                    searchForProduct(company, branches);
                    break;
                case "2":
                    listOfProducts(company, branches);
                    break;
                case "3":
                    previousOrders();
                    break;
                case "4":
                    goBack = true;
                    break;
                default:
                    System.err.println("Invalid input.");
                    break;
            }
        }
    }

    /**
     * @param company Company which products are searched in
     * @param branches They contain products. They are belong to company class
     */
    public void searchForProduct(Company company, FurnitureBranch[] branches) {
        Scanner scanner = new Scanner(System.in); // Initializing scanner
        FurnitureBranch branch; // Branch that customer choose
        int userInput;          // User's choice

        // Branch Menu
        for(int i = 0; i<company.getBranchNumber(); i++) {
            System.out.println( (i+1) + ") Branch " + (i+1));
        }
        System.out.print("Enter: ");

        // User input
        userInput = scanner.nextInt() - 1;
        branch = branches[userInput];

        // Products
        Furniture[] productArr;        // product will be removed from this array
        Model       productModel;      // model of the product which user chose
        Color       productColor;      // color of the product which user chose
        int         productAmount = 0; // shows how many available products are there for the specified product

        // User Input
        scanner = new Scanner(System.in);
        System.out.print("Product name  : ");
        productArr = branch.getFurnitureArr(scanner.nextLine());
        System.out.print("Product model : ");
        productModel = Model.getModel(scanner.next());
        System.out.print("Product color : ");
        productColor = Color.getColor(scanner.next());

        // Product amount
        try {
            productAmount = branch.getCount(productArr, productColor, productModel);
        } catch (NullPointerException n) {
            System.err.println("Product you are looking for doesn't exist.");
        }

        // Getting customer's information
        if(productAmount == 0)
            System.out.println("Sorry! The product you are looking for is out of stock.");
        else {
            // address
            scanner = new Scanner(System.in);
            System.out.print("Address : ");
            setAddress(scanner.nextLine());

            // phone number
            while(true) {
                try {
                    System.out.print("Phone number : ");
                    // user input
                    setPhoneNumber(scanner.nextLong());
                    // break if doesn't throw exception
                    break;

                } catch (Exception E) {
                    System.err.println("Invalid input for phone number.");
                }
            }

            // removing the product from stock
            branch.removeProduct(productArr, productColor, productModel);
            // adding it as previous order
            setOrder(productArr[0].getName(), productModel, productColor);
            // informing the customer
            System.out.println("Process completed. Your information:" +
                    "\nName    : " + getName() +
                    "\nSurname : " + getSurname() +
                    "\nID      : " + getCustomerID() +
                    "\nAddress : " + getAddress() +
                    "\nPhone   : " + getPhoneNumber());
        }

    }

    
    /**
     * @param company Company which products are listed in
     * @param branches They contain products. They are belong to company class
     */
    public void listOfProducts(Company company, FurnitureBranch[] branches) {
        // User Input & Scanner initializing
        Scanner scanner = new Scanner(System.in);
        Furniture[] f = new Furniture[0];
        int branch;
        int furniture, furnitureArr;
        String furnitureName = "";

        // Branch Menu
        for(int i = 0; i<company.getBranchNumber(); i++) {
            System.out.println( (i+1) + ") Branch " + (i+1));
        }
        System.out.print("Enter: ");

        branch = scanner.nextInt() - 1;


        if(branch < 0 && branch >= company.getBranchNumber()) {
            System.err.println("Invalid input");
            return;
        }

        // Furniture Menu
        System.out.println("1) Office Chairs");
        System.out.println("2) Office Desks");
        System.out.println("3) Meeting Tables");
        System.out.println("4) Bookcases");
        System.out.println("5) Office Cabinets");
        System.out.print("Enter: ");

        furnitureArr = scanner.nextInt();

        switch (furnitureArr) {
            case 1:
                branches[branch].printFurnitureArr(branches[branch].officeChairs);
                f = branches[branch].officeChairs;
                furnitureName = "Office Chair";
                break;
            case 2:
                branches[branch].printFurnitureArr(branches[branch].officeDesks);
                f = branches[branch].officeDesks;
                furnitureName = "Office Desk";
                break;
            case 3:
                branches[branch].printFurnitureArr(branches[branch].meetingTables);
                f = branches[branch].meetingTables;
                furnitureName = "Meeting Table";
                break;
            case 4:
                branches[branch].printFurnitureArr(branches[branch].bookcases);
                f = branches[branch].bookcases;
                furnitureName = "Bookcase";
                break;
            case 5:
                branches[branch].printFurnitureArr(branches[branch].officeCabinets);
                f = branches[branch].officeCabinets;
                furnitureName = "Office Cabinet";
                break;
            default:
                System.err.println("Invalid input.");
                break;
        }

        // User Input
        System.out.print("Enter the number of the product that you want to buy: ");
        scanner = new Scanner(System.in);
        furniture = scanner.nextInt() - 1;
        if(furniture < 0 || furniture >= f.length)
            System.err.println("Invalid choice!");
        else {
            // address
            scanner = new Scanner(System.in);
            System.out.print("Address : ");
            setAddress(scanner.nextLine());

            // phone number
            while(true) {
                try {
                    System.out.print("Phone number : ");
                    // user input
                    setPhoneNumber(scanner.nextLong());
                    // break if doesn't throw exception
                    break;

                } catch (Exception E) {
                    System.err.println("Invalid input for phone number.");
                }
            }

            // informing the customer
            System.out.println("Process completed. Your information:" +
                    "\nName    : " + getName() +
                    "\nSurname : " + getSurname() +
                    "\nID      : " + getCustomerID() +
                    "\nMail    : " + getEmail() +
                    "\nAddress : " + getAddress() +
                    "\nPhone   : " + getPhoneNumber() +
                    "\nProduct : " + furnitureName + " - " + f[furniture].getModel() + " - " + f[furniture].getColor());



            // adding as a previous order
            setOrder(furnitureName, f[furniture].getModel(), f[furniture].getColor());
            // removing product
            branches[branch].removeProduct(f, f[furniture].getColor(), f[furniture].getModel());
        }



    }

    
    /**
     * Shows previous orders of the customer. Then returns to menu.
     */
    public void previousOrders() {
        System.out.println("\nPrevious Orders");
        System.out.println("---------------");
        for(int i=0; i<getOrderCount(); i++)
            System.out.println( (i+1) + ") " + getOrder(i));
        System.out.println("---------------\n");
    }

    /**
     * Prints out personal information of the customer.
     */
    @Override
    public String toString() {
        String customer = "Information:" +
                "\nName    : " + getName() +
                "\nSurname : " + getSurname() +
                "\nMail    : " + getEmail() +
                "\nID      : " + getCustomerID() + "\n";

        return customer;
    }
}
