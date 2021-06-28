package com.tgknyhn.homework1;

import java.util.Scanner;

/**
 * Automation system interface for a company
 */
public interface Company {
    // Max Branch number
    /**
     * Maximum branch number this company can contain
     */
    int MAX_BRANCH_NUMBER   = 3;
    /**
     * Maximum employee number this company can contain
     */
    int MAX_EMPLOYEE_NUMBER = 3;
    /**
     * Maximum customer number this company can contain
     */
    int MAX_CUSTOMER_NUMBER = 3;

    // [Getters]

    /**
     * Returns Furniture Branch with from branches array with given index
     * @return Furniture Branch
     */
    tkLinkedList<FurnitureBranch> getBranches();

    /**
     * Returns name of the company
     * @return Name of the company
     */
    String getName();

    /**
     * Returns number of branches
     * @return Number of branches
     */
    int getBranchNumber();

    /**
     * Returns administrator of the company
     * @return Administrator of the company
     */
    User getAdmin();

    /**
     * Returns total number of employees in the company
     * @return Number of employees in the company
     */
    int getEmployeeNumber();

    /**
     * Returns total number of customers in the company
     * @return Total number of customers
     */
    int getCustomerNumber();

    /**
     * Returns arraylist of employees
     * @return ArrayList of employees
     */
    tkArrayList<User> getEmployees();

    /**
     * Returns arraylist of customers
     * @return Arraylist of customers
     */
    tkArrayList<User> getCustomers();

    // [Setters]

    /**
     * Changes company's name with given parameter
     * @param name New name of the company
     */
    void setName(String name);

    /**
     * Changes company's branch size with given parameter
     * @param size New number of branches of the company
     */
    void setBranchNumber(int size);

    /**
     * Changes admin of the company with given parameter
     * @param admin New admin of the company
     */
    void setAdmin(User admin);

    /**
     * Changes total employee number in the company with given size
     * @param number Number of employee
     */
    void setEmployeeNumber(int number);

    /**
     * Changes total customer number in the company with given number
     * @param number Customer number
     */
    void setCustomerNumber(int number);


    // [Automation]


    /**
     * User Login choice screen for different type of users
     */
    default void userLogin() {
        // Scanner & inputs
        Scanner scanner = new Scanner(System.in);
        String email;
        String password;

        System.out.print("Email   : ");
        email = scanner.next();
        System.out.print("Password: ");
        password = scanner.next();

        if(email.contains("admin"))
            adminLogin(email, password);
        else if(email.contains("employee"))
            employeeLogin(email, password);
        else if(email.contains("customer"))
            customerLogin(email, password);
        else
            System.err.println("Invalid input! Login was unsuccessful");
    }

    /**
     * Adds an user to the system with given parameters
     * @param userType Type of the user(Admin, Customer, Employee)
     * @param name Name of the user
     * @param surname Surname of the user
     * @param email Email of the user
     * @param password Password of the user
     */
    default void userRegister(String userType, String name, String surname, String email, String password) {
        System.out.println("*** REGISTRATION ***");
        System.out.println("--------------------");

        // Scanner & inputs
        System.out.println("Name     : " + name);
        System.out.println("Surname  : " + surname);
        System.out.println("Email    : " + email);
        System.out.println("Password : " + password);


        if(!email.contains("@" + userType + ".com"))
            System.err.println("Invalid email. Mail should finish with @" + userType + ".com");

        try {
            if(isThereSuchUser(email, password, userType))
                System.err.println("There is already an account with the same e-mail.");
            else
                switch (userType) {
                    case "employee":
                        employeeRegister(name, surname, email, password);
                        break;
                    case "customer":
                        customerRegister(name, surname, email, password);
                        break;
                    default:
                        System.err.println("User couldn't added.");
                        break;
                }
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.println("Registration failed! You are adding more " + userType + " than you could. Max : 3");
        }

    }

    /**
     * Return true if there is a registered User with given parameters. Else false
     * @param email Email of the User
     * @param password Password of the User
     * @return True if user is in the system. Else false.
     */
    boolean isThereSuchUser(String email, String password, String userType);

    /**
     * Returns the Customer with the given ID
     * @param ID ID of the customer
     * @return Customer which ID is same as parameter
     */
    User findCustomerByID(int ID);

    // [Login]

    /**
     * Logs admin into the system if given email and password is correct
     * @param email Email of admin
     * @param password Password of admin
     */
    void adminLogin(String email, String password);

    /**
     * Logs employee into the system if given email and password is correct
     * @param email Email of employee
     * @param password Password of employee
     */
    void employeeLogin(String email, String password);

    /**
     * Logs customer into the system if given email and password is correct
     * @param email Email of customer
     * @param password Password of customer
     */
    void customerLogin(String email, String password);

    // [Register]

    /**
     * Adds an employee with given parameters to the system. Only admins can use it.
     * @param name Name of the employee
     * @param surname Surname of the employee
     * @param email Email of the employee
     * @param password Password of the employee
     */
    void employeeRegister(String name, String surname, String email, String password);

    /**
     * Adds a customer with given parameters to the system. Only Branch Employees and customers can use it.
     * @param name Name of the customer
     * @param surname Surname of the customer
     * @param email Email of the customer
     * @param password Password of the customer
     */
    void customerRegister(String name, String surname, String email, String password);


}
