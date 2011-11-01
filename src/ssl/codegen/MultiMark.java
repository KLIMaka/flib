package ssl.codegen;

import java.util.ArrayList;
import java.util.Iterator;

public class MultiMark implements Iterable<Mark>
{

    private ArrayList<Mark> m_marks = new ArrayList<Mark>();

    public MultiMark(Mark mark)
    {
        addMark(mark);
    }

    public void addMark(Mark mark)
    {
        m_marks.add(mark);
    }

    @Override
    public Iterator<Mark> iterator()
    {
        return m_marks.iterator();
    }
}
