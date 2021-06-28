package com.tgknyhn.homework1;

/**
 * Color enumerator for a furniture. Contains 6 colors.
 */
public enum Color {
    RED, WHITE, BLACK, PINK, BLUE, DEFAULT;

    /**
     * Returns the equivalent color of the given string parameter as enum
     * @param name Name of the color
     * @return Color as enum
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
