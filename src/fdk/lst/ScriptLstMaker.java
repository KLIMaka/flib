package fdk.lst;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ScriptLstMaker implements IEntryMaker {

    static public class Entry extends BasicEntryMaker.Entry {

        public Entry(String val, int idx) {
            super(val, idx);
        }

        public String getName() {
            return getValue().trim();
        }

        public String getComment() {
            return super.getComment().replaceFirst("#.*", "").trim();
        }

        public int getLocalVars() {
            Pattern p = Pattern.compile(".*# local_vars=(\\d*)");
            Matcher m = p.matcher(super.getComment());
            if (m.find()) {
                return Integer.parseInt(m.group(1));
            }
            return 0;
        }

        public void setName(String name) {
            while (name.length() < 16)
                name += " ";
            setContent(name + ";" + getComment());
        }

        public void setComment(String comm) {
            comm = comm.trim();
            if (comm.length() > 46) {
                comm = comm.substring(0, 45);
            } else {
                while (comm.length() < 46)
                    comm += " ";
            }
            setContent(getContent().replaceFirst(";.*#", "; " + comm + "#"));
        }

        public void setLocalVars(int lv) {
            String lvs = String.valueOf(lv);
            setContent(getContent().replaceFirst("# local_vars=(\\d*)", "# local_vars=" + lvs));
        }

        @Override
        public String toString() {
            return getIndex() + " " + getName() + " " + getComment();
        }

    }

    @Override
    public IEntry create(int index, BufferedReader reader) throws IOException {
        if (!reader.ready()) return null;

        return new Entry(reader.readLine(), index);
    }

}
