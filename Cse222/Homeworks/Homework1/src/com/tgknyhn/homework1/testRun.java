package com.tgknyhn.homework1;

import java.util.Scanner;

/**
 * Driver class for the automation system
 */
public class testRun {
    public static void main(String[] args){

        // Admin
        User admin = new Administrator("tugkan", "ayhan", "tugkan@admin.com", "1234");


        // Company
        Company company = new FurnitureCompany("ABC", 3, admin);

        // Scanner & Input
        Scanner scanner = new Scanner(System.in);
        String input = "";

        // loop condition
        boolean didQuit = false;
         // Shows a selectable menu for every kind of user (Administrator, customer, etc.)
        while(!didQuit) {
            // Menu
            System.out.println("Company name      : " + company.getName());
            System.out.println("Number of branches: " + company.getBranchNumber());
            System.out.println("1) Login");
            System.out.println("2) Customer Registration");
            System.out.println("3) Quit");
            System.out.print("Enter: ");
            // User Input
            input = scanner.next();

            switch (input) {
                case "1":
                    company.userLogin();
                    break;
                case "2":
                    company.userRegister("customer");
                    break;
                case "3":
                    System.out.println("Good Bye!");
                    didQuit = true;
                    break;
                default:
                    System.err.println("Invalid input!");
                    break;
            }
        }


        /*
        FurnitureBranch branch = new FurnitureBranch(5,4,3,2,1);

        System.out.println(branch);

        Furniture OC  = new OfficeChair(Color.BLACK, Model.v1);
        Furniture OD  = new OfficeDesk(Color.PINK, Model.v3);
        Furniture MT  = new MeetingTable(Color.RED, Model.v2);
        Furniture BC  = new Bookcase(Model.v12);
        Furniture OCB = new OfficeCabinet(Model.v12);

        System.out.println(OC);
        System.out.println(OD);
        System.out.println(MT);
        System.out.println(BC);
        System.out.println(OCB);
        */
    }
}
