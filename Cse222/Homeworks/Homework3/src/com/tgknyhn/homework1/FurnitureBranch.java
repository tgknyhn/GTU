package com.tgknyhn.homework1;

/**
 * Class for a branch of a Furniture company. Contains all necessary methods that a furniture branch requires.
 */
public class FurnitureBranch {
    // Max Product
    /**
     * Maximum office chair number branch can contain
     */
    final public static int MAX_CHAIR    = 7*5;  // 7 model, 5 color and 1 product for each of them

    /**
     * Maximum office desk number branch can contain
     */
    final public static int MAX_DESK     = 5*4;  // 5 model, 4 color and 1 product for each of them

    /**
     * Maximum meeting table number branch can contain
     */
    final public static int MAX_TABLE    = 10*4; // 10 model, 4 color and 1 product for each of them

    /**
     * Maximum bookcase number branch can contain
     */
    final public static int MAX_BOOKCASE = 12;   // 12 model and 1 product for each of them

    /**
     * Maximum office cabinet number branch can contain
     */
    final public static int MAX_CABINET  = 12;   // 12 model and 1 product for each of them

    // furniture
    /**
     * HybridList for office chairs. Holds all office chairs this branch contains.
     */
    public tkHybridList<Furniture> officeChairs;
    /**
     * HybridList for office desks. Holds all office desks this branch contains.
     */
    public tkHybridList<Furniture> officeDesks;
    /**
     * HybridList for meeting tables. Holds all meeting tables this branch contains.
     */
    public tkHybridList<Furniture> meetingTables;
    /**
     * HybridList for bookcases. Holds all bookcases this branch contains.
     */
    public tkHybridList<Furniture> bookcases;
    /**
     * HybridList for office cabinets. Holds all office cabinets this branch contains.
     */
    public tkHybridList<Furniture> officeCabinets;


    /**
     * The base constructor which initializes all properties for a Furniture Branch
     */
    public FurnitureBranch() {
        // allocating furniture's size and their first product
        officeChairs   = new tkHybridList<>();          officeChairs.add(new OfficeChair(OfficeChair.colorsOC[0], OfficeChair.modelsOC[0]));
        officeDesks    = new tkHybridList<>();          officeDesks.add(new OfficeDesk(OfficeDesk.colorsOD[0], OfficeDesk.modelsOD[0]));
        meetingTables  = new tkHybridList<>();          meetingTables.add(new MeetingTable(MeetingTable.colorsMT[0], MeetingTable.modelsMT[0]));
        bookcases      = new tkHybridList<>();          bookcases.add(new Bookcase(Bookcase.modelsBC[0]));
        officeCabinets = new tkHybridList<>();          officeCabinets.add(new OfficeCabinet(OfficeCabinet.modelsOCB[0]));

        // filling Furniture Arrays
        furnitureArrFiller(officeChairs, MAX_CHAIR);
        furnitureArrFiller(officeDesks, MAX_DESK);
        furnitureArrFiller(meetingTables, MAX_TABLE);
        furnitureArrFiller(bookcases, MAX_BOOKCASE);
        furnitureArrFiller(officeCabinets, MAX_CABINET);
    }

    /**
     * Fills given furniture list with proper furnitures
     * @param f Furniture list
     * @param maxFurniture Limit for the furniture list
     */
    private void furnitureArrFiller(tkHybridList<Furniture> f, int maxFurniture) {
        for(int i=0, c=0, m=0; i<maxFurniture; i++, c++) {
            // reset color index
            if(c == f.get(0).colors.length){
                c = 0;
                m++;
            }
            // reset model index
            if(m == f.get(0).models.length)
                m = 0;

            // allocating the Furniture
            switch (f.get(0).getName()) {
                case "Office Chair":
                    f.add(new OfficeChair(f.get(0).colors[c], f.get(0).models[m]));
                    break;
                case "Office Desk":
                    f.add(new OfficeDesk(f.get(0).colors[c], f.get(0).models[m]));
                    break;
                case "Meeting Table":
                    f.add(new MeetingTable(f.get(0).colors[c], f.get(0).models[m]));
                    break;
                case "Bookcase":
                    f.add(new Bookcase(f.get(0).models[m]));
                    break;
                case "Office Cabinet":
                    f.add(new OfficeCabinet(f.get(0).models[m]));
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
    public int getCount(tkHybridList<Furniture> f, Color c, Model m) {
        int count = 0;
        for (int i=0; i<f.size(); i++)
            if(f.get(i) != null)
                if (f.get(i).getColor() == c && f.get(i).getModel() == m)
                    count++;

        return count;
    }

    /**
     * Returns HybridList of the furnitures
     * @param name type of the furniture
     * @return HybridList of the selected furnitures
     */
    public tkHybridList<Furniture> getFurnitureArr(String name) {
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
    public int getFurnitureCount(tkHybridList<Furniture> f) {
        int amount, count = 0;
        for(Model m : f.get(0).models) {
            for(Color c : f.get(0).colors) {
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
    public void printFurnitureArr(tkHybridList<Furniture> f) {
        int amount = 0, index = 0;
        // Furniture
        for(Model m : f.get(0).models) {
            for(Color c : f.get(0).colors) {
                try {
                    amount = getCount(f, c, m);
                } catch (NullPointerException n) {
                    amount = 0;
                }

                index++;

                if(amount == 0)
                    System.out.println(index + ")\t" + f.get(0).getName() + "\t(Model: " + m + ")\t(Color: " + c + ") \t(Amount: " + amount + ") ! Out of stock");
                else
                    System.out.println(index + ")\t" + f.get(0).getName() + "\t(Model: " + m + ")\t(Color: " + c + ") \t(Amount: " + amount + ")");
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
    public void addProduct(tkHybridList<Furniture> f, Color c, Model m) {
        // allocating new furniture
        Furniture furniture = null;
        // deciding furniture type
        switch (f.get(0).getName()) {
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
        for(int i=0; i<f.size(); i++)
            if(f.get(i) == null)
                f.set(i, furniture);
    }

    /**
     * Removes one product from the furniture array with given color and model
     * @param f Furniture array
     * @param c Color will be removed
     * @param m Model will be removed
     */
    public void removeProduct(tkHybridList<Furniture> f, Color c, Model m) {
        // Copying furniture array
        for(int i=0; i<f.size(); i++) {
            try {
                // skipping selected furniture
                if(f.get(i).getModel() == m && f.get(i).getColor() == c)
                    f.set(i, null);
            } catch (NullPointerException ignored) { }
        }
    }

    /**
     * Adds one product for all out of stock furniture
     */
    public void addAllProducts(tkHybridList<Furniture> f) {
        int amount = 0, count = 0;
        for(Model m : f.get(0).models) {
            for(Color c : f.get(0).colors) {
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
