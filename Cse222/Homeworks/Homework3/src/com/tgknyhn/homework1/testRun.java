package com.tgknyhn.homework1;

import java.util.Scanner;

public class testRun {
    private static final Administrator admin   = new Administrator("tugkan", "ayhan", "tugkan@admin.com", "1234");
    private static final Company company       = new FurnitureCompany("ABC", 3, admin);
    private static BranchEmployee employee;
    private static Customer customer;

    // LOGIN
    private static String email = "";
    private static String password = "";

    // REGISTER
    private static String regName     = "";
    private static String regSurname  = "";
    private static String regEmail    = "";
    private static String regPassword = "";

    public static void main(String[] args){

        // Admin
        email = "tugkan@admin.com";
        password = "1234";

        // Employee register information
        regName     = "ahmet";
        regSurname  = "ayhan";
        regEmail    = "e2@employee.com";
        regPassword = "12345";

        /* [ADMIN COMMANDS] */

        // MENU (login part) - admin
        menu(1);

        // Add Branch
        try { admin.addBranch(company, company.getBranches()); }
        catch (ArrayIndexOutOfBoundsException e) { System.out.println("You are adding more than you could. (Max Branch: 3)"); }

        // Remove Branch
        try {
            for(int i=3; i>=0; i--)
                admin.removeBranch(company, company.getBranches(), Integer.toString(i));
        } catch (ArrayIndexOutOfBoundsException e) { System.out.println("There is no branch left"); }

        // Add Branch
        try { admin.addBranch(company, company.getBranches()); }
        catch (ArrayIndexOutOfBoundsException e) { System.out.println("You are adding more than you could. (Max Branch: 3)"); }

        // Remove Branch Employee
        try { admin.removeBranchEmployee(company, company.getEmployees(), "1"); }
        catch (ArrayIndexOutOfBoundsException e) { System.out.println("There is no employee left"); }

        // Add Branch Employee
        try { admin.addBranchEmployee(company, regName, regSurname, regEmail, regPassword); }
        catch (ArrayIndexOutOfBoundsException e) { System.out.println("You are adding more than you could. (Max Employee: 3)"); }

        // Remove Branch Employee
        try {
            for(int i=0; i<2; i++)
                admin.removeBranchEmployee(company, company.getEmployees(), "1"); }
        catch (ArrayIndexOutOfBoundsException e) { System.out.println("There is no employee left\n"); }

        // Add Branch Employee
        try { admin.addBranchEmployee(company, regName, regSurname, regEmail, regPassword); System.out.println("");}
        catch (ArrayIndexOutOfBoundsException e) { System.out.println("You are adding more than you could. (Max Employee: 3)"); }

        // Control Products
        admin.controlProducts(company, company.getBranches(), 1);
        System.out.println("");

        /* [EMPLOYEE COMMANDS] */

        email = "e2@employee.com";
        password = "12345";

        // getting employee
        employee = (BranchEmployee) company.getEmployees().get(0);

        // MENU (login part) - employee
        menu(1);

        // Inquire Stock
        employee.inquireStock(company, company.getBranches(), 0, 2);


        // Customer Information - Error
        try { employee.customerInformation(company, company.getBranches(), 9143, 1, 1); }
        catch (ArrayIndexOutOfBoundsException a) { System.err.println("Branch array out of bound"); }

        // Customer Information - Add New Order
        try { employee.customerInformation(company, company.getBranches(), 1000, 1, 1); }
        catch (ArrayIndexOutOfBoundsException a) { System.err.println("Branch array out of bound"); }
        employee.addNewOrder(company.getBranches().get(0), company.findCustomerByID(1000), "Office Chair", Model.v1, Color.RED);
        employee.addNewOrder(company.getBranches().get(0), company.findCustomerByID(1000), "Office Cair", Model.v1, Color.RED); // INVALID

        // Customer Information - Previous orders
        try { employee.customerInformation(company, company.getBranches(), 1000, 2, 1); }
        catch (ArrayIndexOutOfBoundsException a) { System.err.println("Branch array out of bound"); }

        // Create Subscription - Customer
        company.userRegister("customer", "sahi", "baki", "sahi@customer.com", "sahi123");
        company.userRegister("customer", "elma", "armut", "elma@customer.com", "elma321");
        company.userRegister("customer", "kirmizi", "shirt", "sahi@customer.com", "123asd"); // FAILS


        /* [EMPLOYEE COMMANDS] */

        email = "sahi@customer.com";
        password = "sahi123";

        customer = (Customer) company.getCustomers().get(1);

        // MENU (login part) - customer
        menu(1);

        // Search For Product
        customer.searchForProduct(company.getBranches(), 1, "Office Cabinet", Model.v2, Color.DEFAULT, "kocaeli", "552233");

        // List of Products
        customer.listOfProducts(company, company.getBranches(), 1, 4, 10, "istanbul", "123532");

        // Previous Orders
        customer.previousOrders();

        // MENU (register part)
        menu(2);
    }

    private static void menu(int input) {
        // Menu
        System.out.println("****** MAIN MENU ******");
        System.out.println("Company name      : " + company.getName());
        System.out.println("Number of branches: " + company.getBranchNumber());
        System.out.println("1) Login");
        System.out.println("2) Customer Registration");
        System.out.println("3) Quit");
        System.out.print("Enter: ");

        if(input < 1 || input > 3) {
            Scanner scanner = new Scanner(System.in);
            input = scanner.nextInt();
        }
        else
            System.out.println(input + "\n");

        if(input == 1)
            userLogin(email, password);
        else if(input == 2)
            company.userRegister("customer", regName, regSurname, regEmail, regPassword);
        else if(input == 3) {
            System.out.println("Good Bye!");
        }
        else
            System.err.println("Invalid Input");
    }

    private static void userLogin(String email, String password) {

        // Scanner & inputs
        Scanner scanner = new Scanner(System.in);

        System.out.println("****** LOGIN ******");
        System.out.println("Email   : " + email);
        System.out.println("Password: " + password);

        if(email.contains("admin"))
            company.adminLogin(email, password);
        else if(email.contains("employee"))
            company.employeeLogin(email, password);
        else if(email.contains("customer"))
            company.customerLogin(email, password);
        else
            System.err.println("Invalid input! Login was unsuccessful");
    }
}
