package ssl.generator.namespace;

import java.util.ArrayList;
import java.util.Iterator;

public class NameSpace implements Iterable<Name> {

    protected ArrayList<Name> m_space      = new ArrayList<Name>();
    private int               m_currentPos = 6;

    public Name addName(String name) {
        Name tmp = getName(name);
        if (tmp != null) {
            return tmp;
        }

        Name nse = new Name(name, m_currentPos);
        m_space.add(nse);
        m_currentPos += nse.getLength() + 2;

        return nse;
    }

    public Name getName(String name) {
        for (Name nse : m_space) {
            if (nse.getName().equalsIgnoreCase(name)) {
                return nse;
            }
        }
        return null;
    }

    @Override
    public Iterator<Name> iterator() {
        return new Iterator<Name>() {

            private int m_index = 0;

            @Override
            public boolean hasNext() {
                return m_index < m_space.size();
            }

            @Override
            public Name next() {
                return m_space.get(m_index++);
            }

            @Override
            public void remove() {
                m_space.remove(m_index);
            }
        };
    }

}
