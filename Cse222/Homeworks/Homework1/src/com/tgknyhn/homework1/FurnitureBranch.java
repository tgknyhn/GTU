package com.tgknyhn.homework1;

import java.nio.file.NotLinkException;

/**
 * It is used with Furniture Company class. It represents furniture company's branches. 
 * It contains all necessary methods and fields for a branch.
 */
public class FurnitureBranch {
    // Max Product
    /**
     * Max Possible chair for a branch
     */
    final public static int MAX_CHAIR    = 7*5;  // 7 model, 5 color and 1 product for each of them
    
    /**
     * Max Possible desk for a branch
     */
    final public static int MAX_DESK     = 5*4;  // 5 model, 4 color and 1 product for each of them
    
    /**
     * Max Possible table for a branch
     */
    final public static int MAX_TABLE    = 10*4; // 10 model, 4 color and 1 product for each of them
    
    /**
     * Max Possible bookcase for a branch
     */
    final public static int MAX_BOOKCASE = 12;   // 12 model and 1 product for each of them
    
    /**
     * Max Possible cabinet for a branch
     */
    final public static int MAX_CABINET  = 12;   // 12 model and 1 product for each of them


    /**
     * OC, OD, MT, BC, OCB = 5
     */
    public static int FURNITURE_SIZE = 5;
    // furniture
    /**
     * Holds office chairs in the branch. They can be sold
     */
    public Furniture[] officeChairs;
    /**
     * Holds office desks in the branch. They can be sold
     */
    public Furniture[] officeDesks;
    /**
     * Holds meeting tables in the branch. They can be sold
     */
    public Furniture[] meetingTables;
    /**
     * Holds bookcases in the branch. They can be sold
     */
    public Furniture[] bookcases;
    /**
     * Holds office cabinets in the branch. They can be sold
     */
    public Furniture[] officeCabinets;


    /**
     * The base constructor which initializes all properties for a Furniture Branch
     */
    public FurnitureBranch() {
        // allocating furniture's size and their first product
        officeChairs   = new OfficeChair[MAX_CHAIR];      officeChairs  [0] = new OfficeChair(OfficeChair.colorsOC[0], OfficeChair.modelsOC[0]);
        officeDesks    = new OfficeDesk[MAX_DESK];        officeDesks   [0] = new OfficeDesk(OfficeDesk.colorsOD[0], OfficeDesk.modelsOD[0]);
        meetingTables  = new MeetingTable[MAX_TABLE];     meetingTables [0] = new MeetingTable(MeetingTable.colorsMT[0], MeetingTable.modelsMT[0]);
        bookcases      = new Bookcase[MAX_BOOKCASE];      bookcases     [0] = new Bookcase(Bookcase.modelsBC[0]);
        officeCabinets = new OfficeCabinet[MAX_CABINET];  officeCabinets[0] = new OfficeCabinet(OfficeCabinet.modelsOCB[0]);
        // filling Furniture Arrays
        furnitureArrFiller(officeChairs);
        furnitureArrFiller(officeDesks);
        furnitureArrFiller(meetingTables);
        furnitureArrFiller(bookcases);
        furnitureArrFiller(officeCabinets);
    }

    private void furnitureArrFiller(Furniture[] f) {
        for(int i=0, c=0, m=0; i<f.length; i++, c++) {
            // reset color index
            if(c == f[0].colors.length){
                c = 0;
                m++;
            }
            // reset model index
            if(m == f[0].models.length)
                m = 0;


            // allocating the Furniture
            switch (f[0].getName()) {
                case "Office Chair":
                    f[i] = new OfficeChair(f[0].colors[c], f[0].models[m]);
                    break;
                case "Office Desk":
                    f[i] = new OfficeDesk(f[0].colors[c], f[0].models[m]);
                    break;
                case "Meeting Table":
                    f[i] = new MeetingTable(f[0].colors[c], f[0].models[m]);
                    break;
                case "Bookcase":
                    f[i] = new Bookcase(f[0].models[m]);
                    break;
                case "Office Cabinet":
                    f[i] = new OfficeCabinet(f[0].models[m]);
                    break;
            }
        }
    }

    /**
     * Returns number of furniture which includes given color and model
     * @param f Furniture array to check
     * @param c Wanted color
     * @param m Wanted model
     * @return Number of furniture which includes given color and model
     */
    public int getCount(Furniture[] f, Color c, Model m) {
        int count = 0;
        for (int i=0; i<f.length; i++)
            if(f[i] != null)
                if (f[i].getColor() == c && f[i].getModel() == m)
                    count++;

        return count;
    }

    public Furniture[] getFurnitureArr(String name) {
        switch (name) {
            case "Office Chair":
                return officeChairs;
            case "Office Desk":
                return officeDesks;
            case "Meeting Table":
                return meetingTables;
            case "Bookcase":
                return bookcases;
            case "Office Cabinet":
                return officeCabinets;
            default:
                return null;
        }
    }


    /**
     * Returns number of available furniture in the array
     * @param f Furniture array
     * @return Number of available furniture
     */
    public int getFurnitureCount(Furniture[] f) {
        int amount, count = 0;
        for(Model m : f[0].models) {
            for(Color c : f[0].colors) {
                try {
                    amount = getCount(f, c, m);
                } catch (NullPointerException n) {
                    amount = 0;
                }

                count += amount;
            }
        }

        return count;
    }

    /*
    public Furniture findFurnitureByIndex(Furniture[] f, int index) {
        int currIndex = 0;
        Furniture furniture;
        // Furniture
        for(Model m : f[0].models) {
            for(Color c : f[0].colors) {
                if(currIndex == index)
                    return f[i]


                currIndex++;

            }
        }
    }
    */

    /**
     * Prints available furniture products with their properties to the console
     * @param f Furniture Array
     */
    public void printFurnitureArr(Furniture[] f) {
        int amount = 0, index = 0;
        // Furniture
        for(Model m : f[0].models) {
            for(Color c : f[0].colors) {
                try {
                    amount = getCount(f, c, m);
                } catch (NullPointerException n) {
                    amount = 0;
                }

                index++;

                if(amount == 0)
                    System.out.println(index + ")\t" + f[0].getName() + "\t(Model: " + m + ")\t(Color: " + c + ") \t(Amount: " + amount + ") ! Out of stock");
                else
                    System.out.println(index + ")\t" + f[0].getName() + "\t(Model: " + m + ")\t(Color: " + c + ") \t(Amount: " + amount + ")");
            }
        }
        System.out.print("\n");
    }

    /**
     * Adds a product to the furniture array with given color and model.
     * @param f Furniture Array
     * @param c Color will be added
     * @param m Model will be added
     */
    public void addProduct(Furniture[] f, Color c, Model m) {
        // allocating new furniture
        Furniture furniture = null;
        // deciding furniture type
        switch (f[0].getName()) {
            case "Office Chair":
                furniture = new OfficeChair(c, m);
                break;
            case "Office Desk":
                furniture = new OfficeDesk(c, m);
                break;
            case "Meeting Table":
                furniture = new MeetingTable(c, m);
                break;
            case "Bookcase":
                furniture = new Bookcase(m);
                break;
            case "Office Cabinet":
                furniture = new OfficeCabinet(m);
                break;
            default:
                System.err.println("Couldn't add the product.");
                break;
        }
        if(furniture == null)
            return;

        // assigning new furniture to the null element
        for(int i=0; i<f.length; i++)
            if(f[i] == null)
                f[i] = furniture;
    }

    /**
     * Removes one product from the furniture array with given color and model
     * @param f Furniture array
     * @param c Color will be removed
     * @param m Model will be removed
     */
    public void removeProduct(Furniture[] f, Color c, Model m) {
        // Temporary Furniture array
        Furniture[] temp = f;
        Furniture removed = null;
        // Copying furniture array
        for(int i=0; i<f.length; i++) {
            try {
                // skipping selected furniture
                if(f[i].getModel() == m && f[i].getColor() == c)
                    f[i] = null;
                else
                    f[i] = temp[i];
            } catch (NullPointerException ignored) { }
        }
    }

    /**
     * Adds one product for all out of stock furniture
     */
    public void addAllProducts(Furniture[] f) {
        int amount = 0, count = 0;
        for(Model m : f[0].models) {
            for(Color c : f[0].colors) {
                try {
                    amount = getCount(f, c, m);
                } catch (NullPointerException n) {
                    amount = 0;
                }

                if(amount == 0) {
                    count++;
                    addProduct(f, c, m);
                }

            }
        }

        if(count != 0)
            System.out.println("Adding was successful. " + count + " product added in total.");
        else
            System.out.println("Stock is already full. No product is added.");
    }
}
