package fdk.msg;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CharsetEncoder;
import java.nio.charset.UnmappableCharacterException;
import java.util.ArrayList;
import java.util.Map;
import java.util.TreeMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import fdk.util.Utils;

/**
 * ����� ��� ������ *msg ������ � ����������� ��������� � �����������.
 * <p>
 * ������ ������ ������� �� ����, �������������� � ���, ���������:
 * 
 * <li><b>������������ �����������</b> - ���� ����� �� ������ ����� ���
 * ������������ ���������� ����������� ��������� � �� ������ ������� '{'
 * <li><b>�����</b> - �����, ���������������� ���������, ����� ����
 * �������������, �������� � '{' � '}'
 * <li><b>����</b> - ������������ ��������, �����������, ����� ACM ����
 * �����������, ��� ������������� ������� ���������. �������� ������ � ��������
 * � "���������� ��������". �������� � '{' � '}'
 * <li><b>���������</b> - ����� ���������, �������� � '{' � '}'
 * <li><b>����������� �����������</b> - �����, ��������� ����� ����������
 * ������� '}' � �� ����� ������� ������
 * <p>
 * ����� �� ������������ ������������ ������� �������, � ��� ������������ �
 * ������ ������� ������������� ��������� ������ (�������������
 * {@link #put(MsgEntry)})
 * 
 * @author KLIMaka
 */
public class MSG {

    /**
     * ������ �������, ������������� � ��� �������, � ������� ��� ����������� �
     * �����
     */
    ArrayList<MsgEntry>    m_list = new ArrayList<MsgEntry>();

    /**
     * ����������� ������� ������� �� �� ����������
     */
    Map<Integer, MsgEntry> m_map  = new TreeMap<Integer, MsgEntry>();

    /**
     * ������ MSG ���� �� �������� ������ � ��������� ����������.
     * 
     * @param stream
     *            ������� �����, �� �������� �������� ������
     * @param encoding
     *            ���������, � ������� �������� ������ MSG ����.
     * @throws IOException
     *             ��� ������� ������ �� ������
     * @throws WrongMsgEncoding
     *             ��� ������������� ������������� ������ �� �������� ������ �
     *             �������������� ��������� ���������
     */
    public MSG(InputStream stream, Charset encoding) throws IOException, WrongMsgEncoding {
        read(stream, encoding);
    }

    /**
     * ������� ������ MSG ����.
     */
    public MSG() {}

    /**
     * ������ �� ������ ������ MSG ����� � ��������� ����������.
     * 
     * ����� ������ ������������ � UTF-16 � ������������ � ��������� ����������.
     * ��� ������������� ���������������� ����� ������ ��� ����� - ��� ������ ��
     * ����������� � ������ �������. ��� ���������� ������� ����������
     * {@link #put(MsgEntry)}
     * 
     * @param in
     *            ������� �����
     * @throws IOException
     *             ��� ������� ������ �� ������
     * @throws WrongMsgEncoding
     *             ��� ������������� ������������� ������ �� �������� ������ �
     *             �������������� ��������� ���������
     */
    public void read(InputStream in, Charset encoding) throws IOException, WrongMsgEncoding {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        Utils.copyStrream(in, out);
        ByteBuffer buf = ByteBuffer.wrap(out.toByteArray());
        CharsetDecoder cd = encoding.newDecoder();
        CharBuffer cb = null;

        try {
            cb = cd.decode(buf);
        } catch (UnmappableCharacterException e) {
            throw new WrongMsgEncoding("Unable to read with current encoding (" + encoding + ")");
        }

        Pattern p = Pattern
                .compile("([^\\{]*)\\{([^\\}]*)\\}[^\\{]*\\{([^\\}]*)\\}[^\\{]*\\{([^\\}]*)\\}([^\\}\\r?\\n]*)?(\\r?\\n)?");
        Matcher matcher = p.matcher(cb);
        while (matcher.find()) {
            String precomment = matcher.group(1);
            String number = matcher.group(2);
            String anim = matcher.group(3);
            String msg = matcher.group(4);
            String postcomment = matcher.group(5);

            int num = 0;
            try {
                num = new Integer(number);
            } catch (NumberFormatException e) {
                continue;
            }

            put(new MsgEntry(num, anim, msg, precomment, postcomment));
        }

    }

    /**
     * ������ �� ������ ������ MSG ����� � ��������������� ��������� ��
     * ���������.
     * 
     * @see #read(InputStream, Charset)
     * @param in
     *            ������� �����
     * @throws IOException
     *             ��� ������� ������ �� ������
     * @throws WrongMsgEncoding
     *             ��� ������������� ������������� ������ �� �������� ������ �
     *             �������������� ��������� ���������
     */
    public void read(InputStream in) throws IOException, WrongMsgEncoding {
        read(in, Charset.defaultCharset());
    }

    /**
     * ����� � ����� ������ MSG ����� � ��������� ���������.
     * 
     * 
     * @param out
     *            �������� �����, � ������� ����� �������� ������
     * @param encoding
     *            ���������, � ������� ����� �������� ������
     * @throws IOException
     *             ��� ������� ������ � �������� �����
     * @throws WrongMsgEncoding
     *             ��� ������������� ���������� �������� ����� � ��������
     *             ���������
     */
    public void write(OutputStream out, Charset encoding) throws IOException, WrongMsgEncoding {
        StringBuilder str = new StringBuilder();
        for (MsgEntry rec : m_list)
            str.append(rec + "\r\n");

        CharsetEncoder encoder = encoding.newEncoder();
        ByteBuffer buf = null;
        try {
            buf = encoder.encode(CharBuffer.wrap(str));
        } catch (CharacterCodingException e) {
            throw new WrongMsgEncoding("Unable to write with current encoding (" + encoding + ")");
        }

        out.write(buf.array());
    }

    /**
     * ����� � ����� ������ MSG ����� � ��������� �� ���������.
     * 
     * @see #write(OutputStream, Charset)
     * @param out
     *            �������� �����
     * @throws IOException
     *             ��� ������� ������ � �������� �����
     * @throws WrongMsgEncoding
     *             ��� ������������� ���������� �������� ����� � ��������
     *             ���������
     */
    public void write(OutputStream out) throws IOException, WrongMsgEncoding {
        write(out, Charset.defaultCharset());
    }

    /**
     * ���������� ��������� � ������ �������.
     * 
     * @param num
     *            ����� ���������
     * @return ������� ���������, ��� null, ���� ��������� � ����� ������� ��
     *         ����������
     */
    public MsgEntry get(int num) {
        return m_map.get(num);
    }

    /**
     * ��������� ����� ���������.
     * 
     * ��� ������ ����������� � �����, � ���� ����� ������ �� ��������, �� ��
     * ������� ������ �������� ����� ������ ��������� ������ � ������ �������.
     * 
     * <pre>
     * //�������� ����
     * //{100}{}{ msg1 }
     * //{101}{}{ msg2 }
     * 
     * msg.get(100) // ������ msg2
     * msg.put( new MsgRec( 100, "", " msg3 ", "", "")); // �� �������������� ������������
     * msg.get(100) // ������ msg3
     * </pre>
     * 
     * @param msg
     *            ����������� ���������
     */
    public void put(MsgEntry msg) {
        m_list.add(msg);
        m_map.put(msg.getNumber(), msg);
    }

    /**
     * ������� ��������� � ��������� �������.
     * 
     * ���� ����� ���������� ��������� ��� �� ��������, �� �� ������� ������
     * ����� �������� ��������� ��������� � ����� �������.
     * 
     * @param num
     *            ����� ������
     * @return ��������� �������������� � ������ ������� ����� ��������, ���
     *         <code>null</code> ���� ������ ��������� ���
     */
    public MsgEntry remove(int num) {
        MsgEntry msg = get(num);
        m_list.remove(msg);
        m_map.remove(num);

        MsgEntry ret = null;
        for (MsgEntry rec : m_list) {
            if (rec.getNumber() == num) m_map.put(num, rec);
        }
        return ret;

    }

}
