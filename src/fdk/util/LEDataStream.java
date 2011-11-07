package fdk.util;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;

public class LEDataStream {

    private DataInputStream m_stream;
    private byte[]          m_buffer = new byte[256];

    public LEDataStream(InputStream s) {
        m_stream = new DataInputStream(s);
    }

    public int readInt() throws IOException {
        // m_stream.read(m_buffer, 0, 4);
        // return (m_buffer[0] & 0xff) | ((m_buffer[1] & 0xff) << 8)
        // | ((m_buffer[2] & 0xff) << 16) | ((m_buffer[3] & 0xff) << 24);
        return Integer.reverseBytes(m_stream.readInt());
    }

    public long readUnsignedInt() throws IOException {
        return readInt() & 0xffffffffL;
    }

    public short readShort() throws IOException {
        // m_stream.read(m_buffer, 0, 2);
        // return (short) ((m_buffer[0] & 0xff) | ((m_buffer[1] & 0xff) << 8));
        return Short.reverseBytes(m_stream.readShort());
    }

    public int readUnsignedShort() throws IOException {
        return readShort() & 0xffff;
    }

    public byte readByte() throws IOException {
        return (byte) m_stream.read();
    }

    public int read() throws IOException {
        return m_stream.read();
    }

    public String readString(int len) throws IOException {
        if (m_buffer.length < len) m_buffer = new byte[len];
        m_stream.read(m_buffer, 0, len);
        return new String(m_buffer, 0, len);
    }

    public int read(byte[] b) throws IOException {
        return m_stream.read(b);
    }

    public int read(byte[] b, int off, int len) throws IOException {
        return m_stream.read(b, off, len);
    }

    public void readFully(byte[] b) throws IOException {
        m_stream.readFully(b);
    }

    public void readFully(byte[] b, int off, int len) throws IOException {
        m_stream.readFully(b, off, len);
    }
}
