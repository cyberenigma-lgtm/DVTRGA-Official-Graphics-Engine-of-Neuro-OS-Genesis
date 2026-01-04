package es.neuroos.dvtrga;

import android.view.Surface;

/**
 * DVTRGA - Direct Visual Transport & Render Graphics Architecture
 * 
 * Copyright (C) 2025 José Manuel Moreno Cano
 * Todos los derechos reservados. All rights reserved.
 * 
 * @author José Manuel Moreno Cano
 * @email admin@neuro-os.es
 * @web https://neuro-os.es
 */
public class DVTRGA {
    
    // Cargar biblioteca nativa
    static {
        System.loadLibrary("dvtrga");
    }
    
    // Colores predefinidos
    public static final int COLOR_BLACK   = 0xFF000000;
    public static final int COLOR_BLUE    = 0xFF0000FF;
    public static final int COLOR_GREEN   = 0xFF00FF00;
    public static final int COLOR_CYAN    = 0xFF00FFFF;
    public static final int COLOR_RED     = 0xFFFF0000;
    public static final int COLOR_MAGENTA = 0xFFFF00FF;
    public static final int COLOR_YELLOW  = 0xFFFFFF00;
    public static final int COLOR_WHITE   = 0xFFFFFFFF;
    
    // Métodos nativos
    public native int init(Surface surface, int width, int height);
    public native void clear(int color);
    public native void putPixel(int x, int y, int color);
    public native void fillRect(int x, int y, int w, int h, int color);
    public native void drawLine(int x1, int y1, int x2, int y2, int color);
    public native void present();
    public native void close();
}
