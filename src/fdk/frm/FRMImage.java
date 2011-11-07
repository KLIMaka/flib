package fdk.frm;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;

public class FRMImage {

    private int      m_framesPerDirection;
    private short    m_xShift[] = new short[6];
    private short    m_yShift[] = new short[6];
    private FRMFrame m_frames[][];

    public FRMImage(InputStream stream) throws IOException {
        DataInputStream s = new DataInputStream(stream);
        checkSignature(s);
        loadDetails(s);
    }

    private void checkSignature(DataInputStream stream) throws IOException {
        if (stream.readInt() != 4) {
            throw new IOException("Invalid FRM signature");
        }
    }

    private void loadDetails(DataInputStream stream) throws IOException {
        stream.readInt();
        m_framesPerDirection = stream.readUnsignedShort();

        for (int i = 0; i < 6; i++)
            m_xShift[i] = stream.readShort();

        for (int i = 0; i < 6; i++)
            m_yShift[i] = stream.readShort();

        int offsets[] = new int[6];
        for (int i = 0; i < 6; i++)
            offsets[i] = stream.readInt();

        int frameAreaSize = stream.readInt();

        int sides = 1;
        if (offsets[1] == 0)
            m_frames = new FRMFrame[1][];
        else {
            m_frames = new FRMFrame[6][];
            sides = 6;
        }

        for (int side = 0; side < sides; side++) {
            m_frames[side] = new FRMFrame[m_framesPerDirection];

            for (int i = 0; i < m_framesPerDirection; i++) {
                int w = stream.readUnsignedShort();
                int h = stream.readUnsignedShort();
                int frameSize = stream.readInt();
                short dx = stream.readShort();
                short dy = stream.readShort();
                byte data[] = new byte[frameSize];
                stream.readFully(data);

                m_frames[side][i] = new FRMFrame(w, h, data, dx, dy);
            }
        }
    }

    public FRMFrame[] getFrames(int side) {
        if (side < 0 || side > 5 || !isRotable()) return m_frames[0];
        return m_frames[side];
    }

    public boolean isRotable() {
        return m_frames.length > 1;
    }
}
