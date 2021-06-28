package com.tgknyhn.homework1;

import java.util.InputMismatchException;
import java.util.Scanner;

/**
 * Implements User interface. It contains all necessary methods and fields for a Branch Employee to help customers and maintain stock.
 */
public class BranchEmployee implements User {
    private String name, surname;
    private String email;
    private String password;

    /**
     * Only constructor for Branch Employee. It takes name, surname, email and password as parameters
     * and initializes Branch Employee class with these values.
     * @param name Name of the Branch Employee
     * @param surname Surname of the Branch Employee
     * @param email E-mail of the Branch Employee
     * @param password Password of the Branch Employee
     */
    public BranchEmployee(String name, String surname, String email, String password) {
        setName(name);
        setSurname(surname);
        setEmail(email);
        setPassword(password);
    }

    @Override
    public String getName() { return name; }

    @Override
    public String getSurname() { return surname; }

    @Override
    public String getEmail() { return email; }

    @Override
    public String getPassword() { return password; }

    @Override
    public void setName(String name) { this.name = name; }

    @Override
    public void setSurname(String surname) { this.surname = surname; }

    @Override
    public void setEmail(String email) { this.email = email; }

    @Override
    public void setPassword(String password) { this.password = password; }

    @Override
    public void commands(Company company, FurnitureBranch[] branches, User[] employees) {
        // Scanner
        Scanner scanner = new Scanner(System.in);
        String input = "";
        // loop condition
        boolean goBack = false;

        while(!goBack) {
            System.out.println("1) Inquire product stock");
            System.out.println("2) Customer information");
            System.out.println("3) Create subscription");
            System.out.println("4) Back");
            System.out.print("Enter: ");

            input = scanner.next();

            switch (input) {
                case "1":
                    inquireStock(company, branches);
                    break;
                case "2":
                    customerInformation(company, branches);
                    break;
                case "3":
                    company.userRegister("customer");
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
     * Selectable menu for branches and prints all products of selected branch
     * User can also inform manager about out of stock products and refill them.
     * @param company Company which both admin and employee is working in
     * @param branches All available branches for the company
     */
    public void inquireStock(Company company, FurnitureBranch[] branches) {
        // User Input & Scanner initializing
        int branch, furniture;
        Furniture[] f = null;
        String inquire;
        Scanner scanner = new Scanner(System.in);

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

        furniture = scanner.nextInt();

        switch (furniture) {
            case 1:
                branches[branch].printFurnitureArr(branches[branch].officeChairs);
                f = branches[branch].officeChairs;
                break;
            case 2:
                branches[branch].printFurnitureArr(branches[branch].officeDesks);
                f = branches[branch].officeDesks;
                break;
            case 3:
                branches[branch].printFurnitureArr(branches[branch].meetingTables);
                f = branches[branch].meetingTables;
                break;
            case 4:
                branches[branch].printFurnitureArr(branches[branch].bookcases);
                f = branches[branch].bookcases;
                break;
            case 5:
                branches[branch].printFurnitureArr(branches[branch].officeCabinets);
                f = branches[branch].officeCabinets;
                break;
            default:
                System.err.println("Invalid input.");
                break;
        }
        // Inquire admin
        System.out.println("Inquire manager about products which out of stock? (Yes: y, No: n)");
        System.out.print("Enter: ");
        // input
        inquire = scanner.next();
        if(inquire.equals("y") || inquire.equals("Y"))
            informManager(branches, f, branch);
    }

    /**
     * Informs the manager about the stock which required to be refilled. User can choose adding all out of stock items.
     * @param branches Branch array, includes company's all branches
     * @param furniture Furniture array, includes furniture s which user is selected
     * @param branch Branch number which products inquired
     */
    public void informManager(FurnitureBranch[] branches, Furniture[] furniture, int branch) {
        String userInput;
        Scanner scanner = new Scanner(System.in);

        System.out.println("You informed the manager. Your manager told you that you can refill the branch stock.");
        System.out.println("Do you want the refill? (Yes: y , No: n)");
        System.out.print("Enter: ");
        // user input
        userInput = scanner.next();

        if(userInput.equals("y") || userInput.equals("Y"))
            branches[branch].addAllProducts(furniture);
        else
            System.err.println("Invalid input.");;
    }

    /**
     * Shows a dialog for the User which shows information for the Customer
     * @param company The company which customer is subscribed to
     */
    public void customerInformation(Company company, FurnitureBranch[] branches) throws ArrayIndexOutOfBoundsException{
        Scanner scanner = new Scanner(System.in); // Initializing scanner
        FurnitureBranch branch; // Branch that customer choose
        int ID = 0;         // Customer ID
        int userInput;      // User's choice

        // User input
        System.out.print("Enter Customer ID: ");
        ID = scanner.nextInt();

        // Getting customer
        User customer = company.findCustomerByID(ID);

        if(customer == null) {
            System.err.println("Customer with given ID doesn't exist.");
            return;
        }
        else
            System.out.println(customer);

        // Menu
        System.out.println("1) Add new order");
        System.out.println("2) Previous orders");
        System.out.print("Enter: ");
        // User input
        userInput = scanner.nextInt();

        // Add New Order
        if(userInput == 1) {
            // Branch Menu
            for(int i = 0; i<company.getBranchNumber(); i++) {
                System.out.println( (i+1) + ") Branch " + (i+1));
            }
            System.out.print("Enter: ");
            // User input
            userInput = scanner.nextInt() - 1;
            branch = branches[userInput];
            // Error
            if(userInput >= company.getBranchNumber())
                System.err.println("Invalid input");
            else
                addNewOrder(branch, customer);

        }
        // Previous orders
        else if(userInput == 2) {
            System.out.println("\nPrevious Orders");
            System.out.println("---------------");
            if(customer.getOrderCount() == 0)
                System.out.println("There is no order yet.");
            else {
                for(int i=0; i<customer.getOrderCount(); i++)
                    System.out.println( (i+1) + ") " + customer.getOrder(i));
            }
            System.out.println("---------------\n");

        }
        else
            System.err.println("Invalid input!");
    }

    public void addNewOrder(FurnitureBranch branch, User customer) throws InputMismatchException {
        // scanner
        Scanner scanner = new Scanner(System.in); // Initializing scanner
        // product
        String      productName;     // name of the product which user chose
        Model       productModel;     // model of the product which user chose
        Color       productColor;     // color of the product which user chose
        Furniture[] productArr; // product will be removed from this array
        int         productAmount = 0;  // shows how many available products are there for the specified product


        System.out.print("Product name  : ");
        productName = scanner.nextLine();
        productArr = branch.getFurnitureArr(productName);
        System.out.print("Product model : ");
        productModel = Model.getModel(scanner.next());
        System.out.print("Product color : ");
        productColor = Color.getColor(scanner.next());

        try {
            productAmount = branch.getCount(productArr, productColor, productModel);
        } catch (NullPointerException n) {
            System.err.println("Product array is Null.");
        }

        // getting customer's information
        if(productAmount == 0)
            System.out.println("Sorry! The product you are looking for is out of stock.");
        else {
            // removing the product from stock
            branch.removeProduct(productArr, productColor, productModel);
            // adding as previous order
            customer.setOrder(productName, productModel, productColor);
            // informing the customer
            System.out.println("One \"" + productName + " - " + productModel + " - " + productColor +
                    "\" is removed from the stock.\nYou can give it to the customer. Customer's information:" +
                    "\nName    : " + customer.getName() +
                    "\nSurname : " + customer.getSurname() +
                    "\nEmail   : " + customer.getEmail() +
                    "\nID      : " + customer.getCustomerID());
        }
    }

    /**
     * Prints out personal information of the employee.
     */
    @Override
    public String toString() {
        return "BranchEmployee{" +
                "name='" + name + '\'' +
                ", surname='" + surname + '\'' +
                ", email='" + email + '\'' +
                ", password='" + password + '\'' +
                '}';
    }
}
