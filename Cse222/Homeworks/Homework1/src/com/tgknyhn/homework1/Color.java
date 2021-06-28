package com.tgknyhn.homework1;

/**
 * Enumerator for Furnitures. All colors represent a furniture color. 
 */
public enum Color {
    RED, WHITE, BLACK, PINK, BLUE, DEFAULT;

    /**
     * Returns given color name as a Color type
     * @param name Name of the color
     */
    public static Color getColor(String name) {
        Color color;

        switch (name) {
            case "red":
                color = Color.RED;
                break;
            case "white":
                color = Color.WHITE;
                break;
            case "black":
                color = Color.BLACK;
                break;
            case "pink":
                color = Color.PINK;
                break;
            case "blue":
                color = Color.BLUE;
                break;
            default:
                color = Color.DEFAULT;
                break;
        }

        return color;
    }


}
