package com.tgknyhn.homework1;

/**
 * Implements Company interface. It contains all necessary methods and fields that a furniture company needs in a automation system.  
 */
public class FurnitureCompany implements Company {

    private String name;
    // Branches
    private FurnitureBranch[] branches;
    // [Users]
    private User   admin;
    public  User[] branchEmployees;
    private User[] customers;
    // Size
    private int branchNumber   = 3;
    private int employeeNumber = 1;
    private int customerNumber = 1;
    // Index (for login)
    private int index = 0;

    // [Constructor]
    /**
     * The constructor which includes all properties for a Furniture Company
     * @param name Company's name
     * @param branchNumber Number of branch
     */
    public FurnitureCompany(String name, int branchNumber, User admin) {

        // [Variables]
        setName(name);
        setBranchNumber(branchNumber);
        setAdmin(admin);
        setEmployeeNumber(1);

        // [Branch]
        branches = new FurnitureBranch[MAX_BRANCH_NUMBER];
        // Initializing
        for(int i = 0; i< branchNumber; i++) {
            branches[i] = new FurnitureBranch();
        }

        // [Branch Employees]
        branchEmployees = new BranchEmployee[MAX_EMPLOYEE_NUMBER];
        // Initializing only first one
        branchEmployees[0] = new BranchEmployee("hayri", "basrili", "e1@employee.com" ,"1234");

        // [Customers]
        customers = new Customer[MAX_CUSTOMER_NUMBER];
        // Initializing only first one
        customers[0] = new Customer("cemil", "kira", "c1@customer.com", "1234");
    }


    // [Getters]

    @Override
    public String getName() { return name; }

    @Override
    public User getAdmin() { return admin; }

    @Override
    public FurnitureBranch getBranch(int index) { return branches[index]; }

    @Override
    public int getBranchNumber() { return branchNumber; }

    @Override
    public int getEmployeeNumber() { return employeeNumber; }

    @Override
    public int getCustomerNumber() { return customerNumber; }

    // [Setters]

    @Override
    public void setName(String name) { this.name = name; }

    @Override
    public void setAdmin(User admin) { this.admin = admin; }

    @Override
    public void setBranchNumber(int size) { this.branchNumber = size; }

    @Override
    public void setEmployeeNumber(int number) { employeeNumber = number; }

    @Override
    public void setCustomerNumber(int number) { customerNumber = number; }

    // [Automation]

    @Override
    public boolean isThereSuchUser(String email, String password, String userType) {
        try {
            // admin
            if(getAdmin().getEmail().equals(email) && getAdmin().getPassword().equals(password) && userType.equals("admin"))
                return true;

            // employee
            for(int i=0; i<getEmployeeNumber(); i++)
                if(branchEmployees[i].getEmail().equals(email) && branchEmployees[i].getPassword().equals(password) && userType.equals("employee")) {
                    index = i;
                    return true;
                }

            // customer
            for(int i=0; i<getCustomerNumber(); i++)
                if(customers[i].getEmail().equals(email) && customers[i].getPassword().equals(password) && userType.equals("customer")){
                    index = i;
                    return true;
                }
        } catch (ArrayIndexOutOfBoundsException e) {
            return false;
        }

        return false;
    }

    @Override
    public User findCustomerByID(int ID) {
        for(int i=0; i<getCustomerNumber(); i++) {
            if(customers[i].getCustomerID() == ID)
                return customers[i];
        }

        return null;
    }

    // Login

    @Override
    public void adminLogin(String email, String password) {
        if(isThereSuchUser(email, password, "admin"))
            admin.commands(this, branches, branchEmployees);
        else
            System.err.println("Account couldn't found.");
    }

    @Override
    public void employeeLogin(String email, String password) {
        if(isThereSuchUser(email, password, "employee"))
            branchEmployees[index].commands(this, branches, branchEmployees);
        else
            System.err.println("Account couldn't found.");
    }

    @Override
    public void customerLogin(String email, String password) {
        if(isThereSuchUser(email, password, "customer"))
            customers[index].commands(this, branches, branchEmployees);
        else
            System.err.println("Account couldn't found");
    }

    // Register

    @Override
    public void employeeRegister(String name, String surname, String email, String password) throws ArrayIndexOutOfBoundsException {
        // allocating new employee
        User employee = new BranchEmployee(name, surname, email, password);
        // assigning it into branch employee array
        branchEmployees[getEmployeeNumber()] = employee;
        // increasing employee number
        setEmployeeNumber(getEmployeeNumber() + 1);
        // notifying the user
        System.out.println("Branch employee \"" + name + " " + surname + "\" is added to the system. Number of employees: " + getEmployeeNumber());
    }

    @Override
    public void customerRegister(String name, String surname, String email, String password) throws ArrayIndexOutOfBoundsException {
        // allocating new customer
        Customer customer = new Customer(name, surname, email, password);
        // assigning it into customers array
        customers[getCustomerNumber()] = customer;
        // increasing customer number
        setCustomerNumber(getCustomerNumber() + 1);
        // notifying the user
        System.out.println("Customer \"" + name + " " + surname + "\" is added to the system. Special ID: " + customer.getCustomerID());
    }

}