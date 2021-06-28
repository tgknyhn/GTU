package com.tgknyhn.homework1;

import java.util.Scanner;

/**
 * Implements User interface. It contains all necessary methods and fields for an admin to control a company  
 */
public class Administrator implements User {
    private String name, surname;
    private String email;
    private String password;

    /**
     * Only constructor for Administrator. It takes name, surname, email and password as parameters
     * and initializes Administrator class with these values.
     * @param name Name of the Administrator
     * @param surname Surname of the Administrator
     * @param email E-mail of the Administrator
     * @param password Password of the Administrator
     */
    public Administrator(String name, String surname, String email, String password) {
        setName(name);
        setSurname(surname);
        setEmail(email);
        setPassword(password);
    }

    // [Getters]

    @Override
    public String getName() { return name; }

    @Override
    public String getSurname() { return surname; }

    @Override
    public String getEmail() { return email; }

    @Override
    public String getPassword() { return password; }

    // [Setters]

    @Override
    public void setName(String name) { this.name = name; }

    @Override
    public void setSurname(String surname) { this.surname = surname; }

    @Override
    public void setEmail(String email) { this.email = email; }

    @Override
    public void setPassword(String password) { this.password = password; }

    // [Automation]

    @Override
    public void commands(Company company, FurnitureBranch[] branches, User[] employees) {
        // Scanner
        Scanner scanner = new Scanner(System.in);
        String input = "";
        // loop condition
        boolean goBack = false;

        while(!goBack) {
            System.out.println("1) Add Branch");
            System.out.println("2) Remove Branch");
            System.out.println("3) Add Branch Employee");
            System.out.println("4) Remove Branch Employee");
            System.out.println("5) Control products");
            System.out.println("6) Back");
            System.out.print("Enter: ");

            // User input
            input = scanner.next();

            try {
                switch (input) {
                    case "1":
                        addBranch(company, branches);
                        break;
                    case "2":
                        removeBranch(company, branches);
                        break;
                    case "3":
                        addBranchEmployee(company);
                        break;
                    case "4":
                        removeBranchEmployee(company, employees);
                        break;
                    case "5":
                        controlProducts(company, branches);
                        break;
                    case "6":
                        goBack = true;
                        break;
                    default:
                        System.err.println("\nInvalid input!");
                        break;
                } // switch end
            } catch (ArrayIndexOutOfBoundsException e) {
                System.err.println("You are adding or removing more than you could. (Max Branch:5, Max Employee:3, Max Customer:3)");
            }
        } // while end
    } // commands end

    /**
     * Adds a branch for the company
     * @param branches Branch array (not full)
     */
    public void addBranch(Company company, FurnitureBranch[] branches) throws ArrayIndexOutOfBoundsException {
        // allocating new branch
        FurnitureBranch newBranch = new FurnitureBranch();
        int i = 0;

        try {
            for(i=0; i<branches.length; i++)
                if(branches[i] == null)
                    throw new NullPointerException();

            throw new ArrayIndexOutOfBoundsException();

        } catch (NullPointerException e) {
            // assigning it into the array
            branches[i] = newBranch;
            // notifying the User
            System.out.println("\nBranch " + (i+1) + " added.\n");
            // increasing branch size
            company.setBranchNumber(company.getBranchNumber()+1);
        }
    }

    /**
     * Removes the branch which given it's index as user input
     * @param company Company which one of the branches will be removed
     * @param branches branch array (not full)
     */
    public void removeBranch(Company company, FurnitureBranch[] branches) throws ArrayIndexOutOfBoundsException {
        // User Input & Scanner initializing
        String userInput = "";
        Scanner scanner = new Scanner(System.in);

        if(company.getBranchNumber() == 0)
            throw new ArrayIndexOutOfBoundsException();

        // Branch Menu
        for(int i = 0; i<company.getBranchNumber(); i++) {
            System.out.println( (i+1) + ") Branch " + (i+1));
        }
        System.out.print("Enter: ");

        // Getting user input
        userInput = scanner.next();

        // allocating temp arr
        FurnitureBranch[] temp = branches;

        try {
            // copying branches
            for(int i = 0; i<company.getBranchNumber(); i++) {
                // skipping selected branch
                if(userInput.equals(Integer.toString(i+1)))
                    branches[i] = null;
                else
                    branches[i] = temp[i];
            }
        } catch (Exception ignored) { }

        // decreasing branch size
        company.setBranchNumber(company.getBranchNumber() - 1);

        // notifying the User
        System.out.println("Branch " + userInput + " is removed.");
    }

    /**
     * Adds a branch employee to the company
     * @param company Employee which employee is added
     */
    public void addBranchEmployee(Company company) throws ArrayIndexOutOfBoundsException { company.userRegister("employee"); }

    /**
     * Shows dialog for current employees. Removes selected employee from company
     * @param company Compony which the employee is removed
     * @param employees Employee array
     */
    public void removeBranchEmployee(Company company, User[] employees) throws  ArrayIndexOutOfBoundsException {
        // User Input & Scanner initializing
        String userInput = "";
        Scanner scanner = new Scanner(System.in);
        // used whilst notifying the user
        User removed = new BranchEmployee("", "", "", "");

        if(company.getEmployeeNumber() < 1)
            throw new ArrayIndexOutOfBoundsException();

        // Employees
        for(int i=0; i<company.getEmployeeNumber(); i++) {
            System.out.println( (i+1) + ") Employee " + employees[i].getName() + " " + employees[i].getSurname());
        }
        System.out.print("Enter: ");

        // Getting user input
        userInput = scanner.next();

        // allocating temp arr
        User[] temp = new BranchEmployee[company.MAX_EMPLOYEE_NUMBER];

        // copying employees
        for(int i=0, j=0; i<company.getEmployeeNumber(); i++, j++) {
            // skipping selected employee
            if(userInput.equals(Integer.toString(i+1))) {
                removed = employees[i];
                j--;
                continue;
            }
            temp[j] = employees[i];
        }

        // assigning temp arr into employees arr
        employees = temp;

        // decreasing employee number
        company.setEmployeeNumber(company.getEmployeeNumber() - 1);

        // Notifying the User
        System.out.println("Employee " + removed.getName() + " " + removed.getSurname() + " is removed from the system.");
    }

    /**
     * Counts products that out of stock and informs admin about the number.
     * @param company Company which admin manages
     * @param branches Branches of the company
     */
    public void controlProducts(Company company, FurnitureBranch[] branches) {
        // User Input & Scanner initializing
        int userInput;
        Scanner scanner = new Scanner(System.in);

        // Branch Menu
        for(int i = 0; i<company.getBranchNumber(); i++) {
            System.out.println( (i+1) + ") Branch " + (i+1));
        }
        System.out.print("Enter: ");

        // Getting user input
        userInput = scanner.nextInt();

        FurnitureBranch branch = branches[userInput-1];

        int amount = 0;

        // Office Chairs
        for(Model m : branch.officeChairs[0].models)
            for(Color c : branch.officeChairs[0].colors) {
                try {
                    if(branch.getCount(branch.officeChairs, c, m) == 0)
                        amount++;
                } catch (NullPointerException n) {
                    amount += 0;
                }
            }
        // Office Desks
        for(Model m : branch.officeDesks[0].models)
            for(Color c : branch.officeDesks[0].colors) {
                try {
                    if(branch.getCount(branch.officeDesks, c, m) == 0)
                        amount++;
                } catch (NullPointerException n) {
                    amount += 0;
                }
            }
        // Meeting Tables
        for(Model m : branch.meetingTables[0].models)
            for(Color c : branch.meetingTables[0].colors) {
                try {
                    if(branch.getCount(branch.meetingTables, c, m) == 0)
                        amount++;
                } catch (NullPointerException n) {
                    amount += 0;
                }
            }
        // Bookcases
        for(Model m : branch.bookcases[0].models)
            for(Color c : branch.bookcases[0].colors) {
                try {
                    if(branch.getCount(branch.bookcases, c, m) == 0)
                        amount++;
                } catch (NullPointerException n) {
                    amount += 0;
                }
            }
        // Office Cabinets
        for(Model m : branch.officeCabinets[0].models)
            for(Color c : branch.officeCabinets[0].colors) {
                try {
                    if(branch.getCount(branch.officeCabinets, c, m) == 0)
                        amount++;
                } catch (NullPointerException n) {
                    amount += 0;
                }
            }

        // Informing admin
        System.out.println("In this branch " + amount + " product is out of stock.");
    }

}

