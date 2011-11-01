package fdk.lst;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;

public class BasicEntryMaker implements IEntryMaker
{

    static public class Entry implements IEntry
    {
        private String m_value;
        private int    m_index;

        public Entry(String val, int idx)
        {
            m_value = val;
            m_index = idx;
        }

        @Override
        public String getValue()
        {
            return m_value.replaceFirst(";.*", "").trim();
        }

        @Override
        public int getIndex()
        {
            return m_index + 1;
        }

        protected String getContent()
        {
            return m_value;
        }

        protected void setContent(String cont)
        {
            m_value = cont;
        }

        public String getComment()
        {
            return m_value.replaceFirst(".*;", "");
        }

        @Override
        public void write(BufferedWriter bw) throws IOException
        {
            bw.write(m_value);
        }

        @Override
        public String toString()
        {
            return getIndex() + " " + getValue() + " "
                    + getComment();
        }
    }

    @Override
    public IEntry create(int index, BufferedReader reader) throws IOException
    {
        if (!reader.ready())
            return null;
        Entry ent = new Entry(reader.readLine(), index);
        return ent;
    }
}
