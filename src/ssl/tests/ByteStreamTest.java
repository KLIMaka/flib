package ssl.tests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertArrayEquals;

import java.util.Iterator;
import java.util.Random;

import org.junit.Test;

import ssl.codegen.ByteStream;
import ssl.codegen.Mark;
import ssl.codegen.MultiMark;
import ssl.codegen.Node;
import ssl.generator.OP;

public class ByteStreamTest {

    @Test
    public void testByteStreamInt() {
        ByteStream stream = new ByteStream(2);
        stream.writeShort((short) 0x1234);
        stream.writeByte((byte) 0x00);
        Mark mark = stream.mark();
        assertEquals(mark.getCluster(), 1);
        assertEquals(mark.getOffset(), 1);
    }

    @Test
    public void testByteStream() {
        ByteStream stream = new ByteStream();
        stream.writeBytes(new byte[512]);
        stream.writeByte((byte) 0x00);
        Mark mark = stream.mark();

        assertEquals(mark.getCluster(), 1);
        assertEquals(mark.getOffset(), 1);
    }

    @Test
    public void testWriteByte() {
        ByteStream stream = new ByteStream();
        stream.writeByte((byte) 0);
        stream.writeByte((byte) 1);
        stream.writeByte((byte) 2);
        stream.writeByte((byte) 3);

        Iterator<Byte> i = stream.iterator();
        assertEquals(i.next(), new Byte((byte) 0));
        assertEquals(i.next(), new Byte((byte) 1));
        assertEquals(i.next(), new Byte((byte) 2));
        assertEquals(i.next(), new Byte((byte) 3));

    }

    @Test
    public void testWriteShort() {
        ByteStream stream = new ByteStream();
        stream.writeShort((short) 0x1234);

        Iterator<Byte> i = stream.iterator();
        assertEquals(i.next(), new Byte((byte) 0x12));
        assertEquals(i.next(), new Byte((byte) 0x34));
    }

    @Test
    public void testWriteInt() {
        ByteStream stream = new ByteStream();
        stream.writeInt(0x12345678);

        Iterator<Byte> i = stream.iterator();
        assertEquals(i.next(), new Byte((byte) 0x12));
        assertEquals(i.next(), new Byte((byte) 0x34));
        assertEquals(i.next(), new Byte((byte) 0x56));
        assertEquals(i.next(), new Byte((byte) 0x78));
    }

    @Test
    public void testWriteFlaot() {
        float val = 1234.5678f;
        ByteStream stream = new ByteStream();
        stream.writeFlaot(val);

        Iterator<Byte> i = stream.iterator();
        int float2int = Float.floatToRawIntBits(val);
        float2int = Integer.reverseBytes(float2int);
        assertEquals(i.next(), new Byte((byte) (float2int & 0xff)));
        float2int >>= 8;
        assertEquals(i.next(), new Byte((byte) (float2int & 0xff)));
        float2int >>= 8;
        assertEquals(i.next(), new Byte((byte) (float2int & 0xff)));
        float2int >>= 8;
        assertEquals(i.next(), new Byte((byte) (float2int & 0xff)));
        float2int >>= 8;
    }

    @Test
    public void testWriteBytes() {
        byte[] bytes = { 1, 2, 3, 4 };
        ByteStream stream = new ByteStream();
        stream.writeBytes(bytes);

        Iterator<Byte> i = stream.iterator();
        assertEquals(i.next(), new Byte((byte) 1));
        assertEquals(i.next(), new Byte((byte) 2));
        assertEquals(i.next(), new Byte((byte) 3));
        assertEquals(i.next(), new Byte((byte) 4));
    }

    @Test
    public void testMark() {
        ByteStream stream = new ByteStream();
        Mark mark = stream.mark();
        stream.writeFlaot(12.34f);
        Mark mark2 = stream.mark();

        assertEquals(mark.getCluster(), 0);
        assertEquals(mark.getOffset(), 0);
        assertEquals(mark2.getOffset(), 4);
    }

    @Test
    public void testPatchMarkT() {
        ByteStream stream = new ByteStream();
        stream.writeByte((byte) 0);
        Mark mark = stream.mark();
        stream.writeInt(0x12345678);
        stream.patch(mark, 0x87654321);

        Iterator<Byte> i = stream.iterator();
        assertEquals(i.next(), new Byte((byte) 0));
        assertEquals(i.next(), new Byte((byte) 0x87));
        assertEquals(i.next(), new Byte((byte) 0x65));
        assertEquals(i.next(), new Byte((byte) 0x43));
        assertEquals(i.next(), new Byte((byte) 0x21));
    }

    @Test
    public void testPatchMultiMarkT() {
        ByteStream stream = new ByteStream();
        stream.writeByte((byte) 0);
        MultiMark mark = new MultiMark(stream.mark());
        stream.writeInt(0x12345678);
        mark.addMark(stream.mark());
        stream.writeInt(0x12345678);
        stream.patch(mark, 0x87654321);

        Iterator<Byte> i = stream.iterator();
        assertEquals(i.next(), new Byte((byte) 0));
        assertEquals(i.next(), new Byte((byte) 0x87));
        assertEquals(i.next(), new Byte((byte) 0x65));
        assertEquals(i.next(), new Byte((byte) 0x43));
        assertEquals(i.next(), new Byte((byte) 0x21));
        assertEquals(i.next(), new Byte((byte) 0x87));
        assertEquals(i.next(), new Byte((byte) 0x65));
        assertEquals(i.next(), new Byte((byte) 0x43));
        assertEquals(i.next(), new Byte((byte) 0x21));
    }

    @Test
    public void testGetSize() {
        ByteStream stream = new ByteStream();
        stream.writeBytes(new byte[1024]);
        ByteStream stream2 = new ByteStream(12);
        stream2.writeBytes(new byte[1024]);

        assertEquals(stream.getSize(), 1024);
        assertEquals(stream2.getSize(), 1024);
    }

    @Test
    public void testGetBytes() {
        ByteStream stream = new ByteStream();
        stream.writeInt(0x12345678);
        byte[] orig = { 0x12, 0x34, 0x56, 0x78 };

        assertArrayEquals(stream.getBytes(), orig);

        ByteStream stream2 = new ByteStream();
        byte[] data = new byte[1024];
        stream2.writeBytes(data);

        assertArrayEquals(stream2.getBytes(), data);
    }

    @Test
    public void testLabels() {
        ByteStream stream1 = new ByteStream();
        ByteStream stream2 = new ByteStream();

        stream1.writeShort((short) 0x8012);
        stream2.writeInt(0x0);
        stream1.writeLabel(stream2.getLabel());
        stream2.writeInt(0x0);
        stream1.writeLabel(stream2.getLabel());

        byte[] orig1 = { (byte) 0x80, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        assertArrayEquals(stream1.getBytes(), orig1);

        stream2.writeLabels(0);
        byte[] orig = { (byte) 0x80, 0x12, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08 };
        assertArrayEquals(stream1.getBytes(), orig);
    }

    public Node newNode() {
        return new Node(new ByteStream());
    }

    @Test
    public void testNodes() {

        int size = new Random().nextInt(8096);

        // if([4][size]) then([size][4]) else([size]) = size*3+8

        Node code = newNode();
        Node next = code.addAfter(newNode());

        Node if_expr = code.addAfter(newNode());
        if_expr.get().writeLabel(next.get().getLabel());
        if_expr.get().writeBytes(new byte[size]);

        Node then_st = if_expr.addAfter(newNode());
        then_st.get().writeBytes(new byte[size]);
        then_st.get().writeLabel(next.get().getLabel());

        Node else_st = then_st.addAfter(newNode());
        else_st.get().writeBytes(new byte[size]);

        code = next;

        int addr = 0;
        for (ByteStream node : code.getHead()) {
            node.writeLabels(addr);
            addr += node.getSize();
        }

        int k = size * 3 + 8;
        int f = (if_expr.get().getBytes()[3] & 0xff) + (if_expr.get().getBytes()[2] << 8)
                + (if_expr.get().getBytes()[1] << 16) + (if_expr.get().getBytes()[0] << 24);

        assertEquals(k, f);

        else_st.get().write(OP.ADD);
    }

}
