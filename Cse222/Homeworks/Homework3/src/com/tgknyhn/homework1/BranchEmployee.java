package com.tgknyhn.homework1;

import java.util.InputMismatchException;
import java.util.Scanner;

/**
 * Contains all necessary methods for a Branch Employee. Implements User interface.
 */
public class BranchEmployee implements User {
    private String name, surname;
    private String email;
    private String password;

    /**
     * Changes Branch Employee's information with given parameters
     * @param name Name of the employee
     * @param surname Surname of the employee
     * @param email Email of the employee
     * @param password Password of the employee
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
    public void commands(Company company, tkLinkedList<FurnitureBranch> branches, tkArrayList<User> employees) {
        // Scanner
        Scanner scanner = new Scanner(System.in);
        String input = "";
        // loop condition

        System.out.println("1) Inquire product stock");
        System.out.println("2) Customer information");
        System.out.println("3) Create subscription");
        System.out.println("4) Back");
        System.out.println("Enter: ");

        /*
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
            default:
                System.err.println("Invalid input.");
                break;
        }
        */
    }


    /**
     * Selectable menu for branches and prints all products of selected branch
     * User can also inform manager about out of stock products and refill them.
     * @param company Company which both admin and employee is working in
     * @param branches All available branches for the company
     */
    public void inquireStock(Company company, tkLinkedList<FurnitureBranch> branches, int branch, int furniture) {
        System.out.println("\n**** INQUIRE STOCK ****");
        System.out.println("-----------------------");
        // User Input & Scanner initializing
        tkHybridList<Furniture> f = null;

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
        System.out.println("Enter: " + (furniture));

        switch (furniture) {
            case 1:
                branches.get(branch).printFurnitureArr(branches.get(branch).officeChairs);
                f = branches.get(branch).officeChairs;
                break;
            case 2:
                branches.get(branch).printFurnitureArr(branches.get(branch).officeDesks);
                f = branches.get(branch).officeDesks;
                break;
            case 3:
                branches.get(branch).printFurnitureArr(branches.get(branch).meetingTables);
                f = branches.get(branch).meetingTables;
                break;
            case 4:
                branches.get(branch).printFurnitureArr(branches.get(branch).bookcases);
                f = branches.get(branch).bookcases;
                break;
            case 5:
                branches.get(branch).printFurnitureArr(branches.get(branch).officeCabinets);
                f = branches.get(branch).officeCabinets;
                break;
            default:
                System.err.println("Invalid input.");
                break;
        }

        informManager(branches, f, branch);
    }

    /**
     * Informs the manager about the stock which required to be refilled. User can choose adding all out of stock items.
     * @param branches Branch array, includes company's all branches
     * @param furniture Furniture array, includes furniture s which user is selected
     * @param branch Branch number which products inquired
     */
    public void informManager(tkLinkedList<FurnitureBranch> branches, tkHybridList<Furniture> furniture, int branch) {
        System.out.println("You informed the manager. Your manager told you that you can refill the branch stock.");
        System.out.println("Do you want the refill? (Yes: y , No: n)");
        System.out.println("Enter: y");

        branches.get(branch).addAllProducts(furniture);
    }

    /**
     * Shows a dialog for the User which shows information for the Customer
     * @param company The company which customer is subscribed to
     */
    public void customerInformation(Company company, tkLinkedList<FurnitureBranch> branches, int ID, int userInput, int branchNumber) throws ArrayIndexOutOfBoundsException{
        System.out.println("\n**** CUSTOMER INFORMATION ****");
        System.out.println("------------------------------");
        FurnitureBranch branch; // Branch that customer choose

        // Getting customer
        User customer = company.findCustomerByID(ID);

        if(customer == null) {
            System.out.println("Customer with given ID doesn't exist.");
            return;
        }
        else
            System.out.println(customer);

        // Menu
        System.out.println("1) Add new order");
        System.out.println("2) Previous orders");
        System.out.println("Enter: " + userInput);

        // Add New Order
        if(userInput == 1) {
            branch = branches.get(branchNumber-1);
            // Error
            if(branchNumber > company.getBranchNumber())
                System.err.println("Invalid input");

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

    /**
     * Sells the given product for the given customer. Adds purchase into previous orders.
     * @param branch Branch which customers buys from
     * @param customer Customer that buys a product
     * @param productName Name of the product
     * @param productModel Model of the product
     * @param productColor Color of the product
     */
    public void addNewOrder(FurnitureBranch branch, User customer, String productName, Model productModel, Color productColor) throws InputMismatchException {
        System.out.println("\n**** ADD NEW ORDER ****");
        System.out.println("-----------------------");
        // scanner
        Scanner scanner = new Scanner(System.in); // Initializing scanner
        // product
        tkHybridList<Furniture> productArr; // product will be removed from this array
        int         productAmount = 0;  // shows how many available products are there for the specified product


        System.out.println("\nProduct name  : " + productName);
        productArr = branch.getFurnitureArr(productName);
        System.out.println("Product model : " + productModel);
        System.out.println("Product color : " + productColor);

        try {
            productAmount = branch.getCount(productArr, productColor, productModel);
        } catch (NullPointerException n) {
            System.out.println("Product doesn't exist.");
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
