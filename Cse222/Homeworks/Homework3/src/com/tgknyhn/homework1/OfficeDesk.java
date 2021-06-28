package com.tgknyhn.homework1;

/**
 * Holds all information for an Office Desk. Extends Furniture class
 */
public class OfficeDesk extends Furniture {

    // Office Desk(OD) properties
    private final static String  nameOD   = "Office Desk";
    private final static int     priceOD  = 150;
    /**
     * Possible models for the furniture
     */
    public final static Model[] modelsOD = {Model.v1, Model.v2, Model.v3, Model.v4, Model.v5};
    /**
     * Possible colors for the furniture
     */
    public final static Color[] colorsOD = {Color.BLACK, Color.RED, Color.WHITE, Color.PINK};

    /**
     * Preferred constructor for OfficeDesk class. Includes all properties
     * @param color Color of the office desk
     * @param model Model of the office desk
     */
    public OfficeDesk(Color color, Model model) { super(nameOD, color, colorsOD, model, modelsOD, priceOD); }

}
