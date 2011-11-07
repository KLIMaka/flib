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
 * Класс для чтения *msg файлов с сохранением структуры и коментариев.
 * <p>
 * Каждая запись состоит из пяти, ассоциированых с ней, элементов:
 * 
 * <li><b>предваряющий комментарий</b> - весь текст от начала файла или
 * завершающего коментария предыдущего сообщения и до первоо символа '{'
 * <li><b>номер</b> - число, идентифицируещее сообщение, может быть
 * отрицательным, заключен в '{' и '}'
 * <li><b>звук</b> - опциональный параметр, указывающий, какой ACM файл
 * проигрывать, при использовании данного сообщения. Работает только в диалогах
 * с "говорящими головами". Заключен в '{' и '}'
 * <li><b>сообщкние</b> - текст сообщения, заключен в '{' и '}'
 * <li><b>заключающий комментарий</b> - текст, следующий после последнего
 * символа '}' и до конца текущей строки
 * <p>
 * Класс не обеспечивает уникальности номеров записей, и при дублировании с
 * данным номером ассоциируется последняя запись (дополнительно
 * {@link #put(MsgEntry)})
 * 
 * @author KLIMaka
 */
public class MSG {

    /**
     * Список записей, расположенных в том порядке, в котором они размещаются в
     * файле
     */
    ArrayList<MsgEntry>    m_list = new ArrayList<MsgEntry>();

    /**
     * Отображение номеров записей на их содержание
     */
    Map<Integer, MsgEntry> m_map  = new TreeMap<Integer, MsgEntry>();

    /**
     * Читает MSG файл из входного потока с указанной кодировкой.
     * 
     * @param stream
     *            входной поток, из которого читаются данные
     * @param encoding
     *            кодировка, в которой сохранен данный MSG файл.
     * @throws IOException
     *             при ошибках чтения из потока
     * @throws WrongMsgEncoding
     *             при невозможности декодирования данных из входного потока с
     *             использованием указанной кодировки
     */
    public MSG(InputStream stream, Charset encoding) throws IOException, WrongMsgEncoding {
        read(stream, encoding);
    }

    /**
     * Создает пустой MSG файл.
     */
    public MSG() {}

    /**
     * Читает из потока данные MSG файла с указанной кодировкой.
     * 
     * Байты потока декодируются в UTF-16 в соответствии с выбранной кодировкой.
     * При невозможности интерпретировать номер записи как целое - эта запись не
     * добавляется в список записей. Для добавления записей использует
     * {@link #put(MsgEntry)}
     * 
     * @param in
     *            входной поток
     * @throws IOException
     *             при ошибках чтения из потока
     * @throws WrongMsgEncoding
     *             при невозможности декодирования данных из входного потока с
     *             использованием указанной кодировки
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
     * Читает из потока данные MSG файла с исппользованием кодировки по
     * умолчанию.
     * 
     * @see #read(InputStream, Charset)
     * @param in
     *            входной поток
     * @throws IOException
     *             при ошибках чтения из потока
     * @throws WrongMsgEncoding
     *             при невозможности декодирования данных из входного потока с
     *             использованием указанной кодировки
     */
    public void read(InputStream in) throws IOException, WrongMsgEncoding {
        read(in, Charset.defaultCharset());
    }

    /**
     * Пишет в поток данные MSG файла в указанной кодировке.
     * 
     * 
     * @param out
     *            выходной поток, в который будет записаны данные
     * @param encoding
     *            кодировка, в которой будут записаны данные
     * @throws IOException
     *             при ошибказ записи в выходной поток
     * @throws WrongMsgEncoding
     *             при невозможности кодировать исходный текст в заданную
     *             кодировку
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
     * Пишет в поток данные MSG файла в кодировке по умолчанию.
     * 
     * @see #write(OutputStream, Charset)
     * @param out
     *            выходной поток
     * @throws IOException
     *             при ошибказ записи в выходной поток
     * @throws WrongMsgEncoding
     *             при невозможности кодировать исходный текст в заданную
     *             кодировку
     */
    public void write(OutputStream out) throws IOException, WrongMsgEncoding {
        write(out, Charset.defaultCharset());
    }

    /**
     * Возвращает сообщение с данным номером.
     * 
     * @param num
     *            номер сообщения
     * @return искомое сообщение, или null, если сообщения с таким номером не
     *         существует
     */
    public MsgEntry get(int num) {
        return m_map.get(num);
    }

    /**
     * Добавляет новое сообщение.
     * 
     * Все записи добавляются в конец, и если номер записи не уникален, то по
     * данному номеру доступна стает только последняя запись с данным номером.
     * 
     * <pre>
     * //исходный файл
     * //{100}{}{ msg1 }
     * //{101}{}{ msg2 }
     * 
     * msg.get(100) // вернет msg2
     * msg.put( new MsgRec( 100, "", " msg3 ", "", "")); // не перезаписывает существующие
     * msg.get(100) // вернет msg3
     * </pre>
     * 
     * @param msg
     *            добавляемое сообщение
     */
    public void put(MsgEntry msg) {
        m_list.add(msg);
        m_map.put(msg.getNumber(), msg);
    }

    /**
     * Удаляет сообщение с указанным номером.
     * 
     * Если номер удаляемого сообщения был не уникален, то по данному номеру
     * стает доступно последнее сообщение с таким номером.
     * 
     * @param num
     *            номер записи
     * @return сообщение ассоциированое с данным номером после удаления, или
     *         <code>null</code> если такого сообщения нет
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
