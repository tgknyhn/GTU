package com.tgknyhn.homework1;

/**
 * Contains all necessary methods for an Administrator. Implements User interface.
 */
public class Administrator implements User {
    private String name, surname;
    private String email;
    private String password;

    /**
     * Changes administrator information with given parameters
     * @param name Name of the administrator
     * @param surname Surname of the administrator
     * @param email Email of the administrator
     * @param password Password of the administrator
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
    public void commands(Company company, tkLinkedList<FurnitureBranch> branches, tkArrayList<User> employees) {
        System.out.println("1) Add Branch");
        System.out.println("2) Remove Branch");
        System.out.println("3) Add Branch Employee");
        System.out.println("4) Remove Branch Employee");
        System.out.println("5) Control products");
        System.out.println("6) Back");
        System.out.print("Enter: ");
    }

    /**
     * Adds a branch for the company
     * @param branches Branch array (not full)
     */
    public void addBranch(Company company, tkLinkedList<FurnitureBranch> branches) throws ArrayIndexOutOfBoundsException {
        System.out.print("\n(1) : ");
        // allocating new branch
        FurnitureBranch newBranch = new FurnitureBranch();
        int i = 0;

        try {
            for(i=0; i<branches.size(); i++)
                if(branches.get(i) == null)
                    throw new NullPointerException();

            throw new ArrayIndexOutOfBoundsException();

        } catch (NullPointerException e) {
            // assigning it into the array
            branches.set(i, newBranch);
            // notifying the User
            System.out.println("Branch " + (i+1) + " added.\n");
            // increasing branch size
            company.setBranchNumber(company.getBranchNumber()+1);
        }
    }

    /**
     * Removes the branch which given it's index as user input
     * @param company Company which one of the branches will be removed
     * @param branches branch array (not full)
     */
    public void removeBranch(Company company, tkLinkedList<FurnitureBranch> branches, String userInput) throws ArrayIndexOutOfBoundsException {
        System.out.print("\n(2) : ");

        if(company.getBranchNumber() == 0)
            throw new ArrayIndexOutOfBoundsException();

        try {
            // copying branches
            for(int i = 0; i<company.getBranchNumber(); i++) {
                // skipping selected branch
                if(userInput.equals(Integer.toString(i+1)))
                    branches.set(i, null);
                else
                    branches.set(i, branches.get(i));
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
    public void addBranchEmployee(Company company, String name, String surname, String email, String password) throws ArrayIndexOutOfBoundsException {
        System.out.print("(3) : ");
        company.userRegister("employee", name, surname, email, password);
    }

    /**
     * Shows dialog for current employees. Removes selected employee from company
     * @param company Compony which the employee is removed
     * @param employees Employee array
     */
    public void removeBranchEmployee(Company company, tkArrayList<User> employees, String userInput) throws  ArrayIndexOutOfBoundsException {
        System.out.print("(4) : ");
        // used whilst notifying the user
        User removed = new BranchEmployee("", "", "", "");

        if(company.getEmployeeNumber() < 1)
            throw new ArrayIndexOutOfBoundsException();

        // allocating temp arr
        User[] temp = new BranchEmployee[company.MAX_EMPLOYEE_NUMBER];

        // copying employees
        for(int i=0, j=0; i<company.getEmployeeNumber(); i++, j++) {
            // skipping selected employee
            if(userInput.equals(Integer.toString(i+1))) {
                removed = employees.get(i);
                j--;
                continue;
            }
            employees.set(i, temp[j]);
        }

        // decreasing employee number
        company.setEmployeeNumber(company.getEmployeeNumber() - 1);

        // Notifying the User
        System.out.println("Employee " + removed.getName() + " " + removed.getSurname() + " is removed from the system.\n");
    }

    /**
     * Counts products that out of stock and informs admin about the number.
     * @param company Company which admin manages
     * @param branches Branches of the company
     */
    public void controlProducts(Company company, tkLinkedList<FurnitureBranch> branches, int userInput) {
        System.out.print("(5) : ");

        FurnitureBranch branch = branches.get(userInput-1);

        int amount = 0;

        // Office Chairs
        for(Model m : branch.officeChairs.get(0).models)
            for(Color c : branch.officeChairs.get(0).colors) {
                try {
                    if(branch.getCount(branch.officeChairs, c, m) == 0)
                        amount++;
                } catch (NullPointerException n) {
                    amount += 0;
                }
            }
        // Office Desks
        for(Model m : branch.officeDesks.get(0).models)
            for(Color c : branch.officeDesks.get(0).colors) {
                try {
                    if(branch.getCount(branch.officeDesks, c, m) == 0)
                        amount++;
                } catch (NullPointerException n) {
                    amount += 0;
                }
            }
        // Meeting Tables
        for(Model m : branch.meetingTables.get(0).models)
            for(Color c : branch.meetingTables.get(0).colors) {
                try {
                    if(branch.getCount(branch.meetingTables, c, m) == 0)
                        amount++;
                } catch (NullPointerException n) {
                    amount += 0;
                }
            }
        // Bookcases
        for(Model m : branch.bookcases.get(0).models)
            for(Color c : branch.bookcases.get(0).colors) {
                try {
                    if(branch.getCount(branch.bookcases, c, m) == 0)
                        amount++;
                } catch (NullPointerException n) {
                    amount += 0;
                }
            }
        // Office Cabinets
        for(Model m : branch.officeCabinets.get(0).models)
            for(Color c : branch.officeCabinets.get(0).colors) {
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

