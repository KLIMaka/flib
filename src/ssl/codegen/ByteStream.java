package ssl.codegen;

import java.util.ArrayList;
import java.util.Iterator;

interface AddressCallback
{
    public void setAddress(int addr);
}

class BufferAC implements AddressCallback
{

    private Mark       m_mark;
    private ByteStream m_stream;

    public BufferAC(ByteStream bs, Mark m)
    {
        m_mark = m;
        m_stream = bs;
    }

    @Override
    public void setAddress(int addr)
    {
        m_stream.patch(m_mark, addr);
    }

}

class Label
{
    private Mark            m_mark;
    private AddressCallback m_callback;

    public Label(Mark m)
    {
        m_mark = m;
    }

    public AddressCallback getCallback()
    {
        return m_callback;
    }

    public void setCallback(AddressCallback cb)
    {
        m_callback = cb;
    }

    public Mark getMark()
    {
        return m_mark;
    }
}

public class ByteStream implements Iterable<Byte>, IByteBlock
{

    private int               m_clusterSize          = 512;
    private int               m_currentCluster       = 0;
    private int               m_currentClusterOffset = 0;
    private int               m_clusterSaved         = 0;
    private int               m_offsetSaved          = 0;
    private ArrayList<byte[]> m_data                 = new ArrayList<byte[]>();
    private boolean           isPatch                = false;
    private ArrayList<Label>  m_labels               = new ArrayList<Label>();

    public ByteStream(int clst_size)
    {
        m_clusterSize = clst_size;
        m_data.add(new byte[m_clusterSize]);
    }

    public ByteStream()
    {
        m_data.add(new byte[m_clusterSize]);
    }

    private void startPath()
    {
        isPatch = true;
        m_clusterSaved = m_currentCluster;
        m_offsetSaved = m_currentClusterOffset;
    }

    private void endPath()
    {
        isPatch = false;
        m_currentCluster = m_clusterSaved;
        m_currentClusterOffset = m_offsetSaved;
    }

    private void setOffset(Mark mark)
    {
        m_currentCluster = mark.m_cluster;
        m_currentClusterOffset = mark.m_offset;
    }

    private void grow()
    {
        if (isPatch == false)
        {
            m_data.add(new byte[m_clusterSize]);
        }
        m_currentCluster++;
        m_currentClusterOffset = 0;
    }

    private int bytesLeft()
    {
        return m_clusterSize - m_currentClusterOffset;
    }

    private byte[] intToBytes(int input)
    {
        byte[] conv = new byte[4];
        conv[3] = (byte) (input & 0xff);
        input >>= 8;
        conv[2] = (byte) (input & 0xff);
        input >>= 8;
        conv[1] = (byte) (input & 0xff);
        input >>= 8;
        conv[0] = (byte) input;
        return conv;
    }

    private byte[] shortToBytes(short input)
    {
        byte[] ret = new byte[2];
        ret[1] = (byte) (input & 0xff);
        input >>= 8;
        ret[0] = (byte) input;
        return ret;
    }

    public void writeByte(byte data)
    {
        if (bytesLeft() == 0)
        {
            grow();
        }
        m_data.get(m_currentCluster)[m_currentClusterOffset++] = data;
    }

    public void writeShort(short data)
    {
        writeBytes(shortToBytes(data));
    }

    public void writeInt(int data)
    {
        writeBytes(intToBytes(data));
    }

    public void writeFlaot(float data)
    {
        writeInt(Float.floatToRawIntBits(data));
    }

    public void writeBytes(byte[] data)
    {
        for (byte b : data)
        {
            writeByte(b);
        }
    }

    public <T> void write(T data)
    {
        if (data instanceof Integer)
        {
            writeInt((Integer) data);
        }
        else if (data instanceof Float)
        {
            writeFlaot((Float) data);
        }
        else if (data instanceof Byte)
        {
            writeByte((Byte) data);
        }
        else if (data instanceof Short)
        {
            writeShort((Short) data);
        }
        else if (data instanceof byte[])
        {
            writeBytes((byte[]) data);
        }
        else
        {
            System.out.println("Error type");
        }
    }

    public void writeLabel(Label label)
    {
        Mark mark = mark();
        writeInt(0);
        label.setCallback(new BufferAC(this, mark));
    }

    public Label getLabel()
    {
        Label l = new Label(mark());
        m_labels.add(l);
        return l;
    }

    public void writeLabels(int addr)
    {
        for (Label l : m_labels)
        {
            l.getCallback().setAddress(
                    addr + l.getMark().getCluster() * m_clusterSize
                            + l.getMark().getOffset());
        }
    }

    public Mark mark()
    {
        return new Mark(m_currentCluster, m_currentClusterOffset);
    }

    public <T> void patch(Mark mark, T data)
    {
        patch(new MultiMark(mark), data);
    }

    public <T> void patch(MultiMark marks, T data)
    {
        startPath();
        for (Mark mark : marks)
        {
            setOffset(mark);
            write(data);
        }
        endPath();
    }

    @Override
    public int getSize()
    {
        return m_clusterSize * m_currentCluster + m_currentClusterOffset;
    }

    @Override
    public byte[] getBytes()
    {
        byte[] ret = new byte[getSize()];

        for (int i = 0; i < m_currentCluster; i++)
        {
            System.arraycopy(m_data.get(i), 0, ret, i * m_clusterSize,
                    m_clusterSize);
        }
        System.arraycopy(m_data.get(m_currentCluster), 0, ret, m_clusterSize
                * m_currentCluster, m_currentClusterOffset);

        return ret;
    }

    @Override
    public Iterator<Byte> iterator()
    {
        return new Iterator<Byte>()
        {
            int cluster = 0;
            int offset  = 0;

            @Override
            public boolean hasNext()
            {
                return cluster * m_clusterSize + offset < getSize();
            }

            @Override
            public Byte next()
            {
                if (offset >= m_clusterSize)
                {
                    offset = 0;
                    cluster++;
                }
                return new Byte(m_data.get(cluster)[offset++]);
            }

            @Override
            public void remove()
            {
            }
        };
    }

}
