package com.tgknyhn.homework1;

/**
 *  Contains all necessary methods for a Customer. Implements User interface.
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
     * Changes customer's information with given parameters
     * @param name Name of the customer
     * @param surname Surname of the customer
     * @param email Email of the customer
     * @param password Password of the customer
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

    /**
     * Returns the ID of customer
     * @return ID of the customer
     */
    public int getCustomerID() { return customerID; }

    @Override
    public String getPassword() { return password; }


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
    public void commands(Company company, tkLinkedList<FurnitureBranch> branches, tkArrayList<User> employees) {
        // Menu
        System.out.println("1) Search for a product");
        System.out.println("2) List of products");
        System.out.println("3) Previous orders");
        System.out.println("4) Back");
        System.out.print("Enter: ");

        /*
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
        */
    }

    /**
     * Online buying. Search for the product which it's properties are given as parameters.
     * @param branches Branch List to buy from
     * @param userInput Selected branch
     * @param productName Name of the product
     * @param productModel Model of the product
     * @param productColor Color of the product
     * @param address Address of the customer
     * @param phone Phone number of the customer
     */
    public void searchForProduct(tkLinkedList<FurnitureBranch> branches, int userInput, String productName, Model productModel, Color productColor, String address, String phone) {
        System.out.println("\n**** SEARCH FOR PRODUCT ****");
        System.out.println("----------------------------");

        FurnitureBranch branch; // Branch that customer choose
        // User input
        branch = branches.get(userInput-1);

        // Products
        tkHybridList<Furniture> productArr;  // product will be removed from this array
        int productAmount = 0;               // shows how many available products are there for the specified product

        // User Input
        System.out.println("Product name  : " + productName);
        productArr = branch.getFurnitureArr(productName);
        System.out.println("Product model : " + productModel);
        System.out.println("Product color : " + productColor);

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
            System.out.println("Address : " + address);
            setAddress(address);

            // phone number
            System.out.print("Phone number : ");
            setPhoneNumber(Integer.parseInt(phone));

            // removing the product from stock
            branch.removeProduct(productArr, productColor, productModel);
            // adding it as previous order
            setOrder(productArr.get(0).getName(), productModel, productColor);
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
     * Online buying. Select the product you want to buy.
     * @param company Company which system's is running on.
     * @param branches Branch list for the customer
     * @param branch Selected branch
     * @param furnitureArr Furniture list
     * @param furniture Selected furniture
     * @param address Address of the customer
     * @param phone Phone number of the customer
     */
    public void listOfProducts(Company company, tkLinkedList<FurnitureBranch> branches, int branch, int furnitureArr, int furniture, String address, String phone) {

        System.out.println("\n**** LIST OF PRODUCTS ****");
        System.out.println("----------------------------");

        tkHybridList<Furniture> f = new tkHybridList<>();
        String furnitureName = "";

        branch -= 1;

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
        System.out.println("Enter: " + furnitureArr);

        switch (furnitureArr) {
            case 1:
                branches.get(branch).printFurnitureArr(branches.get(branch).officeChairs);
                f = branches.get(branch).officeChairs;
                furnitureName = "Office Chair";
                break;
            case 2:
                branches.get(branch).printFurnitureArr(branches.get(branch).officeDesks);
                f = branches.get(branch).officeDesks;
                furnitureName = "Office Desk";
                break;
            case 3:
                branches.get(branch).printFurnitureArr(branches.get(branch).meetingTables);
                f = branches.get(branch).meetingTables;
                furnitureName = "Meeting Table";
                break;
            case 4:
                branches.get(branch).printFurnitureArr(branches.get(branch).bookcases);
                f = branches.get(branch).bookcases;
                furnitureName = "Bookcase";
                break;
            case 5:
                branches.get(branch).printFurnitureArr(branches.get(branch).officeCabinets);
                f = branches.get(branch).officeCabinets;
                furnitureName = "Office Cabinet";
                break;
            default:
                System.err.println("Invalid input.");
                break;
        }

        // User Input
        System.out.println("Enter the number of the product that you want to buy: " + furniture);
        furniture -= 1;
        if(furniture < 0 || furniture >= f.size())
            System.err.println("Invalid choice!");
        else {
            // address
            System.out.println("Address : " + address);
            setAddress(address);

            // phone number
            System.out.println("Phone number : " + phone);
            setPhoneNumber(Integer.parseInt(phone));

            // informing the customer
            System.out.println("Process completed. Your information:" +
                    "\nName    : " + getName() +
                    "\nSurname : " + getSurname() +
                    "\nID      : " + getCustomerID() +
                    "\nMail    : " + getEmail() +
                    "\nAddress : " + getAddress() +
                    "\nPhone   : " + getPhoneNumber() +
                    "\nProduct : " + furnitureName + " - " + f.get(furniture).getModel() + " - " + f.get(furniture).getColor());



            // adding as a previous order
            setOrder(furnitureName, f.get(furniture).getModel(), f.get(furniture).getColor());
            // removing product
            branches.get(branch).removeProduct(f, f.get(furniture).getColor(), f.get(furniture).getModel());
        }



    }

    /**
     * Lists previous orders of the customer.
     */
    public void previousOrders() {
        System.out.println("\nPrevious Orders");
        System.out.println("---------------");
        for(int i=0; i<getOrderCount(); i++)
            System.out.println( (i+1) + ") " + getOrder(i));
        System.out.println("---------------\n");
    }

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
