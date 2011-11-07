package fdk.fs.override;

import java.util.Map;
import java.util.TreeMap;

import fdk.fs.IFileEntry;

public class OverrideDir extends OverrideNode implements IFileEntry {

    public OverrideDir(IFileEntry ent) {
        super(ent);
    }

    @Override
    public int getSize() {
        return 0;
    }

    @Override
    public boolean isDirectory() {
        return true;
    }

    @Override
    public IFileEntry[] getSub() {
        Map<String, OverrideNode> ents = new TreeMap<String, OverrideNode>(String.CASE_INSENSITIVE_ORDER);
        for (IFileEntry ent : bottom().getSub()) {
            if (ent.isDirectory())
                ents.put(ent.getName(), new OverrideDir(ent));
            else ents.put(ent.getName(), new OverrideNode(ent));
        }

        for (int i = 1; i < getOverrides().size(); i++) {
            for (IFileEntry ent : getOverrides().get(i).getSub()) {
                OverrideNode node = ents.get(ent.getName());
                if (node == null) {
                    if (ent.isDirectory())
                        ents.put(ent.getName(), new OverrideDir(ent));
                    else ents.put(ent.getName(), new OverrideNode(ent));
                } else {
                    node.addOverride(ent);
                }
            }
        }

        IFileEntry ret[] = new IFileEntry[ents.size()];
        int i = 0;
        for (IFileEntry ent : ents.values())
            ret[i++] = ent;
        return ret;
    }
}
