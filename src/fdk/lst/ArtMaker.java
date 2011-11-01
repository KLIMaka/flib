package fdk.lst;

import java.io.BufferedReader;
import java.io.IOException;

public class ArtMaker implements IEntryMaker
{

    static public class CritterEntry extends BasicEntryMaker.Entry
    {

        public CritterEntry(String val, int idx)
        {
            super(val, idx);
        }

        @Override
        public String getValue()
        {
            return getContent().replaceFirst(",.*", "") + "aa.frm";
        }

    }

    static public class HeadEntry extends BasicEntryMaker.Entry
    {

        public HeadEntry(String val, int idx)
        {
            super(val, idx);
        }

        @Override
        public String getValue()
        {
            return getContent().replaceFirst(",.*", "") + "bf1.frm";
        }

    }

    private int m_type = 0;

    public ArtMaker(String type)
    {
        if (type.equalsIgnoreCase("critters"))
            m_type = 1;
        else if (type.equalsIgnoreCase("heads"))
            m_type = 2;
    }

    @Override
    public IEntry create(int index, BufferedReader reader) throws IOException
    {
        if (!reader.ready())
            return null;

        IEntry ent;
        switch (m_type) {
        case 0:
            ent = new BasicEntryMaker.Entry(reader.readLine(), index);
            break;
        case 1:
            ent = new CritterEntry(reader.readLine(), index);
            break;
        case 2:
            ent = new HeadEntry(reader.readLine(), index);
            break;
        default:
            return null;
        }

        return ent;
    }
}
