package fdk.rix;

import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;

import fdk.util.LEDataStream;

public class RIXImage {

    private int  m_width;
    private int  m_height;
    private byte m_data[];
    private byte m_pallete[];

    public RIXImage(InputStream stream) throws IOException {
        loadImage(new LEDataStream(stream));
    }

    protected void loadImage(LEDataStream stream) throws IOException {
        checkSig(stream);
        loadSize(stream);
        loadPallete(stream);
        loadData(stream);
    }

    protected void checkSig(LEDataStream stream) throws IOException {
        final byte RIX3[] = { 0x52, 0x49, 0x58, 0x33 };
        byte sig[] = new byte[4];
        stream.read(sig);
        if (!Arrays.equals(sig, RIX3)) {
            throw new IOException("Invalid RIX3 signature");
        }
    }

    protected void loadSize(LEDataStream stream) throws IOException {
        m_width = stream.readUnsignedShort();
        m_height = stream.readUnsignedShort();
        stream.readShort();
    }

    protected void loadPallete(LEDataStream stream) throws IOException {
        m_pallete = new byte[256 * 3];
        stream.readFully(m_pallete);
    }

    protected void loadData(LEDataStream stream) throws IOException {
        m_data = new byte[m_width * m_height];
        stream.readFully(m_data);

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

    public byte[] getPallete() {
        return m_pallete;
    }
}
