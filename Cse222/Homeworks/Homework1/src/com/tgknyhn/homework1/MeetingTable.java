package com.tgknyhn.homework1;

/**
 * Implements Furniture interface for a Meeting Table. It contains all properties for an meeting table  
 */
public class MeetingTable extends Furniture {

    // Meeting Table(MT) properties
    private final static String nameMT   = "Meeting Table";
    private final static int    priceMT  = 200;
    /**
     * Possible models for a meeting table
     */
    public  final static Model[] modelsMT = {Model.v1, Model.v2, Model.v3, Model.v4, Model.v5, Model.v6, Model.v7, Model.v8, Model.v9, Model.v10};
    /**
     * Possible colors for a meeting table
     */
    public  final static Color[] colorsMT = {Color.BLACK, Color.RED, Color.WHITE, Color.PINK};
    /**
     * Preferred constructor for MeetingTable class. Includes all properties
     * @param color Color of the Meeting Table
     * @param model MMTel of the Meeting Table
     */
    public MeetingTable(Color color, Model model) { super(nameMT, color, colorsMT, model, modelsMT, priceMT); }

}
