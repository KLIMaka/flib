package fdk.frm;

public class FRMFrame {

    private int   m_width;
    private int   m_height;
    private short m_dx;
    private short m_dy;
    private byte  m_data[];

    public FRMFrame(int w, int h, byte[] data, short dx, short dy) {
        m_width = w;
        m_height = h;
        m_data = data;
        m_dx = dx;
        m_dy = dy;
    }

    public int getWidth() {
        return m_width;
    }

    public int getHeight() {
        return m_height;
    }

    public byte[] getData() {
        return m_data;
    }

    public short getDx() {
        return m_dx;
    }

    public short getDy() {
        return m_dy;
    }
}
