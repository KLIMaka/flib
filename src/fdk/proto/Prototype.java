package fdk.proto;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Map;
import java.util.TreeMap;

public class Prototype
{

    private Map<String, Integer> m_fields = new TreeMap<String, Integer>();

    public Prototype(InputStream in) throws IOException
    {

        try
        {
            DataInputStream din = new DataInputStream(in);

            m_fields.put("protoID", din.readInt());
            m_fields.put("descrID", din.readInt());
            m_fields.put("gndFID", din.readInt());

            int proType = (Integer) m_fields.get("protoID");
            proType >>= 24;
            proType &= 0x0f;

            switch (proType) {
            case 0:
                readItem(din);
                break;
            case 1:
                readCritter(din);
                break;
            case 2:
                readScenery(din);
                break;
            case 3:
                readWall(din);
                break;
            case 4:
                readTile(din);
                break;
            case 5:
                readMisc(din);
                break;

            default:
                throw new IOException("PROTO: Unknown proID (" + proType + ")");
            }
        }
        catch (Exception e)
        {
            throw new IOException("PROTO: " + e.getMessage());
        }
    }

    private void readMisc(DataInputStream din) throws IOException
    {
        m_fields.put("lightDist", din.readInt());
        m_fields.put("lightIntens", din.readInt());

        m_fields.put("flags", din.readInt());
        m_fields.put("flagsExt", din.readInt());
    }

    private void readTile(DataInputStream din) throws IOException
    {
        m_fields.put("lightDist", din.readInt());
        m_fields.put("lightIntens", din.readInt());

        m_fields.put("flags", din.readInt());
        m_fields.put("flagsExt", din.readInt());
    }

    private void readWall(DataInputStream din) throws IOException
    {
        m_fields.put("lightDist", din.readInt());
        m_fields.put("lightIntens", din.readInt());

        m_fields.put("flags", din.readInt());
        m_fields.put("flagsExt", din.readInt());

        m_fields.put("scriptID", din.readInt());

        m_fields.put("matID", din.readInt());
    }

    private void readScenery(DataInputStream din) throws IOException
    {
        m_fields.put("lightDist", din.readInt());
        m_fields.put("lightIntens", din.readInt());

        m_fields.put("flags", din.readInt());
        m_fields.put("flagsExt", din.readInt());

        m_fields.put("scriptID", din.readInt());

        m_fields.put("objSubType", din.readInt());

        m_fields.put("matID", din.readInt());

        m_fields.put("soundID", din.readUnsignedByte());

        switch ((Integer) m_fields.get("objSubType")) {
        case 0:
            readPortal(din);
            break;
        case 1:
            readStairs(din);
            break;
        case 2:
            readElevator(din);
            break;
        case 3:
            readLadderBottom(din);
            break;
        case 4:
            readLadderTop(din);
            break;
        case 5:
            readGenericScenery(din);
            break;

        default:
            throw new IOException("PROTO: Unknown object subtype (" + m_fields.get("objSubType")
                    + ")");
        }
    }

    private void readPortal(DataInputStream din) throws IOException
    {
        m_fields.put("walkThru", din.readInt());
        m_fields.put("unknown", din.readInt());
    }

    private void readStairs(DataInputStream din) throws IOException
    {
        m_fields.put("destPos", din.readInt());
        m_fields.put("destMap", din.readInt());
    }

    private void readElevator(DataInputStream din) throws IOException
    {
        m_fields.put("elevType", din.readInt());
        m_fields.put("elevLevel", din.readInt());
    }

    private void readLadderBottom(DataInputStream din) throws IOException
    {
        m_fields.put("destPos", din.readInt());
    }

    private void readLadderTop(DataInputStream din) throws IOException
    {
        m_fields.put("destPos", din.readInt());
    }

    private void readGenericScenery(DataInputStream din) throws IOException
    {
        m_fields.put("unknown", din.readInt());
    }

    private void readCritter(DataInputStream din) throws IOException
    {
        m_fields.put("lightDist", din.readInt());
        m_fields.put("lightIntens", din.readInt());

        m_fields.put("flags", din.readInt());
        m_fields.put("flagsExt", din.readInt());

        m_fields.put("scriptID", din.readInt());

        m_fields.put("headFID", din.readInt());
        m_fields.put("aiPacket", din.readInt());
        m_fields.put("teamNum", din.readInt());
        m_fields.put("critterFlags", din.readInt());

        m_fields.put("strength", din.readInt());
        m_fields.put("perception", din.readInt());
        m_fields.put("endurance", din.readInt());
        m_fields.put("charisma", din.readInt());
        m_fields.put("intelligence", din.readInt());
        m_fields.put("agility", din.readInt());
        m_fields.put("luck", din.readInt());
        m_fields.put("hp", din.readInt());
        m_fields.put("ap", din.readInt());
        m_fields.put("ac", din.readInt());
        m_fields.put("unarmedDamage", din.readInt());
        m_fields.put("meleeDamage", din.readInt());
        m_fields.put("carryWeight", din.readInt());
        m_fields.put("sequence", din.readInt());
        m_fields.put("healingRate", din.readInt());
        m_fields.put("criticalChance", din.readInt());
        m_fields.put("betterCriticals", din.readInt());

        m_fields.put("DTNormal", din.readInt());
        m_fields.put("DTLaser", din.readInt());
        m_fields.put("DTFire", din.readInt());
        m_fields.put("DTPlasma", din.readInt());
        m_fields.put("DTElectrical", din.readInt());
        m_fields.put("DTEMP", din.readInt());
        m_fields.put("DTExplode", din.readInt());

        m_fields.put("DRNormal", din.readInt());
        m_fields.put("DRLaser", din.readInt());
        m_fields.put("DRFire", din.readInt());
        m_fields.put("DRPlasma", din.readInt());
        m_fields.put("DRElectrical", din.readInt());
        m_fields.put("DREMP", din.readInt());
        m_fields.put("DRExplode", din.readInt());
        m_fields.put("DRRadiation", din.readInt());
        m_fields.put("DRPoison", din.readInt());

        m_fields.put("age", din.readInt());
        m_fields.put("gender", din.readInt());

        m_fields.put("strength+", din.readInt());
        m_fields.put("perception+", din.readInt());
        m_fields.put("endurance+", din.readInt());
        m_fields.put("charisma+", din.readInt());
        m_fields.put("intelligence+", din.readInt());
        m_fields.put("agility+", din.readInt());
        m_fields.put("luck+", din.readInt());
        m_fields.put("hp+", din.readInt());
        m_fields.put("ap+", din.readInt());
        m_fields.put("ac+", din.readInt());
        m_fields.put("unarmedDamage+", din.readInt());
        m_fields.put("meleeDamage+", din.readInt());
        m_fields.put("carryWeight+", din.readInt());
        m_fields.put("sequence+", din.readInt());
        m_fields.put("healingRate+", din.readInt());
        m_fields.put("criticalChance+", din.readInt());
        m_fields.put("betterCriticals+", din.readInt());

        m_fields.put("DTNormal+", din.readInt());
        m_fields.put("DTLaser+", din.readInt());
        m_fields.put("DTFire+", din.readInt());
        m_fields.put("DTPlasma+", din.readInt());
        m_fields.put("DTElectrical+", din.readInt());
        m_fields.put("DTEMP+", din.readInt());
        m_fields.put("DTExplode+", din.readInt());

        m_fields.put("DRNormal+", din.readInt());
        m_fields.put("DRLaser+", din.readInt());
        m_fields.put("DRFire+", din.readInt());
        m_fields.put("DRPlasma+", din.readInt());
        m_fields.put("DRElectrical+", din.readInt());
        m_fields.put("DREMP+", din.readInt());
        m_fields.put("DRExplode+", din.readInt());
        m_fields.put("DRRadiation+", din.readInt());
        m_fields.put("DRPoison+", din.readInt());

        m_fields.put("age+", din.readInt());
        m_fields.put("gender+", din.readInt());

        m_fields.put("smallGuns", din.readInt());
        m_fields.put("bigGuns", din.readInt());
        m_fields.put("energyWeapons", din.readInt());
        m_fields.put("unarmed", din.readInt());
        m_fields.put("melee", din.readInt());
        m_fields.put("throwing", din.readInt());
        m_fields.put("firsAid", din.readInt());
        m_fields.put("doctor", din.readInt());
        m_fields.put("sneak", din.readInt());
        m_fields.put("lockpick", din.readInt());
        m_fields.put("steal", din.readInt());
        m_fields.put("traps", din.readInt());
        m_fields.put("science", din.readInt());
        m_fields.put("repair", din.readInt());
        m_fields.put("speech", din.readInt());
        m_fields.put("barter", din.readInt());
        m_fields.put("gambling", din.readInt());
        m_fields.put("outdoorsman", din.readInt());

        m_fields.put("bodyType", din.readInt());
        m_fields.put("exp", din.readInt());
        m_fields.put("killType", din.readInt());
        m_fields.put("damageType", din.readInt());
    }

    private void readItem(DataInputStream din) throws IOException
    {
        m_fields.put("lightDist", din.readInt());
        m_fields.put("lightIntens", din.readInt());

        m_fields.put("flags", din.readInt());
        m_fields.put("flagsExt", din.readInt());

        m_fields.put("scriptID", din.readInt());

        m_fields.put("objSubType", din.readInt());

        m_fields.put("matID", din.readInt());

        m_fields.put("size", din.readInt());
        m_fields.put("weight", din.readInt());
        m_fields.put("cost", din.readInt());
        m_fields.put("invFID", din.readInt());

        m_fields.put("soundID", din.readUnsignedByte());

        switch ((Integer) m_fields.get("objSubType")) {
        case 0:
            readArmor(din);
            break;
        case 1:
            readContainer(din);
            break;
        case 2:
            readDrug(din);
            break;
        case 3:
            readWeapon(din);
            break;
        case 4:
            readAmmo(din);
            break;
        case 5:
            readMiscItem(din);
            break;
        case 6:
            readKey(din);
            break;

        default:
            throw new IOException("PROTO: Unknown object subtype (" + m_fields.get("objSubType")
                    + ")");
        }
    }

    private void readMiscItem(DataInputStream din) throws IOException
    {
        m_fields.put("powerPID", din.readInt());
        m_fields.put("powerType", din.readInt());
        m_fields.put("charges", din.readInt());
    }

    private void readKey(DataInputStream din) throws IOException
    {
        m_fields.put("unknown", din.readInt());
    }

    private void readAmmo(DataInputStream din) throws IOException
    {
        m_fields.put("caliber", din.readInt());
        m_fields.put("quantity", din.readInt());
        m_fields.put("acAjust", din.readInt());
        m_fields.put("drAjust", din.readInt());
        m_fields.put("dmgMult", din.readInt());
        m_fields.put("dmgDiv", din.readInt());
    }

    private void readWeapon(DataInputStream din) throws IOException
    {
        m_fields.put("animCode", din.readInt());
        m_fields.put("minDmg", din.readInt());
        m_fields.put("maxDmg", din.readInt());
        m_fields.put("dmgType", din.readInt());
        m_fields.put("maxRange1", din.readInt());
        m_fields.put("maxRange2", din.readInt());
        m_fields.put("projFID", din.readInt());
        m_fields.put("minST", din.readInt());
        m_fields.put("apCost1", din.readInt());
        m_fields.put("apCost2", din.readInt());
        m_fields.put("critFail", din.readInt());
        m_fields.put("perk", din.readInt());
        m_fields.put("rounds", din.readInt());
        m_fields.put("caliber", din.readInt());
        m_fields.put("ammoPID", din.readInt());
        m_fields.put("maxAmmo", din.readInt());
        m_fields.put("shotSoundID", din.readUnsignedByte());
    }

    private void readDrug(DataInputStream din) throws IOException
    {
        m_fields.put("stat0", din.readInt());
        m_fields.put("stat1", din.readInt());
        m_fields.put("stat2", din.readInt());

        m_fields.put("0amount0", din.readInt());
        m_fields.put("0amount1", din.readInt());
        m_fields.put("0amount2", din.readInt());

        m_fields.put("1duration", din.readInt());
        m_fields.put("1amount0", din.readInt());
        m_fields.put("1amount1", din.readInt());
        m_fields.put("1amount2", din.readInt());

        m_fields.put("2duration", din.readInt());
        m_fields.put("2amount0", din.readInt());
        m_fields.put("2amount1", din.readInt());
        m_fields.put("2amount2", din.readInt());

        m_fields.put("addictionRate", din.readInt());
        m_fields.put("wEffect", din.readInt());
        m_fields.put("wOnSet", din.readInt());
    }

    private void readContainer(DataInputStream din) throws IOException
    {
        m_fields.put("maxSize", din.readInt());
        m_fields.put("containerFlags", din.readInt());
    }

    private void readArmor(DataInputStream din) throws IOException
    {
        m_fields.put("ac", din.readInt());

        m_fields.put("DRNormal", din.readInt());
        m_fields.put("DRLaser", din.readInt());
        m_fields.put("DRFire", din.readInt());
        m_fields.put("DRPlasma", din.readInt());
        m_fields.put("DRElectrical", din.readInt());
        m_fields.put("DREMP", din.readInt());
        m_fields.put("DRExplode", din.readInt());

        m_fields.put("DTNormal", din.readInt());
        m_fields.put("DTLaser", din.readInt());
        m_fields.put("DTFire", din.readInt());
        m_fields.put("DTPlasma", din.readInt());
        m_fields.put("DTElectrical", din.readInt());
        m_fields.put("DTEMP", din.readInt());
        m_fields.put("DTExplode", din.readInt());

        m_fields.put("perk", din.readInt());
        m_fields.put("maleFID", din.readInt());
        m_fields.put("femaleFID", din.readInt());

    }

    public Map<String, Integer> getFields()
    {
        return m_fields;
    }

    public void write(OutputStream os) throws IOException
    {
        DataOutputStream dos = new DataOutputStream(os);

        dos.writeInt(m_fields.get("protoID"));
        dos.writeInt(m_fields.get("descrID"));
        dos.writeInt(m_fields.get("gndFID"));

        int proType = (Integer) m_fields.get("protoID");
        proType >>= 24;
        proType &= 0x0f;

        switch (proType) {
        case 0:
            writeItem(dos);
            break;
        case 1:
            writeCritter(dos);
            break;
        case 2:
            writeScenery(dos);
            break;
        case 3:
            writeWall(dos);
            break;
        case 4:
            writeTile(dos);
            break;
        case 5:
            writeMisc(dos);
            break;
        }

        dos.close();
    }

    private void writeMisc(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("lightDist"));
        dos.writeInt(m_fields.get("lightIntens"));

        dos.writeInt(m_fields.get("flags"));
        dos.writeInt(m_fields.get("flagsExt"));
    }

    private void writeTile(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("lightDist"));
        dos.writeInt(m_fields.get("lightIntens"));

        dos.writeInt(m_fields.get("flags"));
        dos.writeInt(m_fields.get("flagsExt"));
    }

    private void writeWall(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("lightDist"));
        dos.writeInt(m_fields.get("lightIntens"));

        dos.writeInt(m_fields.get("flags"));
        dos.writeInt(m_fields.get("flagsExt"));

        dos.writeInt(m_fields.get("scriptID"));

        dos.writeInt(m_fields.get("matID"));
    }

    private void writeScenery(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("lightDist"));
        dos.writeInt(m_fields.get("lightIntens"));

        dos.writeInt(m_fields.get("flags"));
        dos.writeInt(m_fields.get("flagsExt"));

        dos.writeInt(m_fields.get("scriptID"));

        dos.writeInt(m_fields.get("objSubType"));

        dos.writeInt(m_fields.get("matID"));

        dos.writeByte(m_fields.get("soundID"));

        switch ((Integer) m_fields.get("objSubType")) {
        case 0:
            writePortal(dos);
            break;
        case 1:
            writeStairs(dos);
            break;
        case 2:
            writeElevator(dos);
            break;
        case 3:
            writeLadderBottom(dos);
            break;
        case 4:
            writeLadderTop(dos);
            break;
        case 5:
            writeGenericScenery(dos);
            break;
        }
    }

    private void writeGenericScenery(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("unknown"));
    }

    private void writeLadderTop(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("destPos"));
    }

    private void writeLadderBottom(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("destPos"));

    }

    private void writeElevator(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("elevType"));
        dos.writeInt(m_fields.get("elevLevel"));
    }

    private void writeStairs(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("destPos"));
        dos.writeInt(m_fields.get("destMap"));
    }

    private void writePortal(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("walkThru"));
        dos.writeInt(m_fields.get("unknown"));
    }

    private void writeCritter(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("lightDist"));
        dos.writeInt(m_fields.get("lightIntens"));

        dos.writeInt(m_fields.get("flags"));
        dos.writeInt(m_fields.get("flagsExt"));

        dos.writeInt(m_fields.get("scriptID"));

        dos.writeInt(m_fields.get("headFID"));
        dos.writeInt(m_fields.get("aiPacket"));
        dos.writeInt(m_fields.get("teamNum"));
        dos.writeInt(m_fields.get("critterFlags"));

        dos.writeInt(m_fields.get("strength"));
        dos.writeInt(m_fields.get("perception"));
        dos.writeInt(m_fields.get("endurance"));
        dos.writeInt(m_fields.get("charisma"));
        dos.writeInt(m_fields.get("intelligence"));
        dos.writeInt(m_fields.get("agility"));
        dos.writeInt(m_fields.get("luck"));
        dos.writeInt(m_fields.get("hp"));
        dos.writeInt(m_fields.get("ap"));
        dos.writeInt(m_fields.get("ac"));
        dos.writeInt(m_fields.get("unarmedDamage"));
        dos.writeInt(m_fields.get("meleeDamage"));
        dos.writeInt(m_fields.get("carryWeight"));
        dos.writeInt(m_fields.get("sequence"));
        dos.writeInt(m_fields.get("healingRate"));
        dos.writeInt(m_fields.get("criticalChance"));
        dos.writeInt(m_fields.get("betterCriticals"));

        dos.writeInt(m_fields.get("DTNormal"));
        dos.writeInt(m_fields.get("DTLaser"));
        dos.writeInt(m_fields.get("DTFire"));
        dos.writeInt(m_fields.get("DTPlasma"));
        dos.writeInt(m_fields.get("DTElectrical"));
        dos.writeInt(m_fields.get("DTEMP"));
        dos.writeInt(m_fields.get("DTExplode"));

        dos.writeInt(m_fields.get("DRNormal"));
        dos.writeInt(m_fields.get("DRLaser"));
        dos.writeInt(m_fields.get("DRFire"));
        dos.writeInt(m_fields.get("DRPlasma"));
        dos.writeInt(m_fields.get("DRElectrical"));
        dos.writeInt(m_fields.get("DREMP"));
        dos.writeInt(m_fields.get("DRExplode"));
        dos.writeInt(m_fields.get("DRRadiation"));
        dos.writeInt(m_fields.get("DRPoison"));

        dos.writeInt(m_fields.get("age"));
        dos.writeInt(m_fields.get("gender"));

        dos.writeInt(m_fields.get("strength+"));
        dos.writeInt(m_fields.get("perception+"));
        dos.writeInt(m_fields.get("endurance+"));
        dos.writeInt(m_fields.get("charisma+"));
        dos.writeInt(m_fields.get("intelligence+"));
        dos.writeInt(m_fields.get("agility+"));
        dos.writeInt(m_fields.get("luck+"));
        dos.writeInt(m_fields.get("hp+"));
        dos.writeInt(m_fields.get("ap+"));
        dos.writeInt(m_fields.get("ac+"));
        dos.writeInt(m_fields.get("unarmedDamage+"));
        dos.writeInt(m_fields.get("meleeDamage+"));
        dos.writeInt(m_fields.get("carryWeight+"));
        dos.writeInt(m_fields.get("sequence+"));
        dos.writeInt(m_fields.get("healingRate+"));
        dos.writeInt(m_fields.get("criticalChance+"));
        dos.writeInt(m_fields.get("betterCriticals+"));

        dos.writeInt(m_fields.get("DTNormal+"));
        dos.writeInt(m_fields.get("DTLaser+"));
        dos.writeInt(m_fields.get("DTFire+"));
        dos.writeInt(m_fields.get("DTPlasma+"));
        dos.writeInt(m_fields.get("DTElectrical+"));
        dos.writeInt(m_fields.get("DTEMP+"));
        dos.writeInt(m_fields.get("DTExplode+"));

        dos.writeInt(m_fields.get("DRNormal+"));
        dos.writeInt(m_fields.get("DRLaser+"));
        dos.writeInt(m_fields.get("DRFire+"));
        dos.writeInt(m_fields.get("DRPlasma+"));
        dos.writeInt(m_fields.get("DRElectrical+"));
        dos.writeInt(m_fields.get("DREMP+"));
        dos.writeInt(m_fields.get("DRExplode+"));
        dos.writeInt(m_fields.get("DRRadiation+"));
        dos.writeInt(m_fields.get("DRPoison+"));

        dos.writeInt(m_fields.get("age+"));
        dos.writeInt(m_fields.get("gender+"));

        dos.writeInt(m_fields.get("smallGuns"));
        dos.writeInt(m_fields.get("bigGuns"));
        dos.writeInt(m_fields.get("energyWeapons"));
        dos.writeInt(m_fields.get("unarmed"));
        dos.writeInt(m_fields.get("melee"));
        dos.writeInt(m_fields.get("throwing"));
        dos.writeInt(m_fields.get("firsAid"));
        dos.writeInt(m_fields.get("doctor"));
        dos.writeInt(m_fields.get("sneak"));
        dos.writeInt(m_fields.get("lockpick"));
        dos.writeInt(m_fields.get("steal"));
        dos.writeInt(m_fields.get("traps"));
        dos.writeInt(m_fields.get("science"));
        dos.writeInt(m_fields.get("repair"));
        dos.writeInt(m_fields.get("speech"));
        dos.writeInt(m_fields.get("barter"));
        dos.writeInt(m_fields.get("gambling"));
        dos.writeInt(m_fields.get("outdoorsman"));

        dos.writeInt(m_fields.get("bodyType"));
        dos.writeInt(m_fields.get("exp"));
        dos.writeInt(m_fields.get("killType"));
        dos.writeInt(m_fields.get("damageType"));
    }

    private void writeItem(DataOutputStream dos) throws IOException
    {

        dos.writeInt(m_fields.get("lightDist"));
        dos.writeInt(m_fields.get("lightIntens"));

        dos.writeInt(m_fields.get("flags"));
        dos.writeInt(m_fields.get("flagsExt"));

        dos.writeInt(m_fields.get("scriptID"));
        dos.writeInt(m_fields.get("objSubType"));
        dos.writeInt(m_fields.get("matID"));

        dos.writeInt(m_fields.get("size"));
        dos.writeInt(m_fields.get("weight"));
        dos.writeInt(m_fields.get("cost"));
        dos.writeInt(m_fields.get("invFID"));

        dos.writeByte(m_fields.get("soundID"));

        switch ((Integer) m_fields.get("objSubType")) {
        case 0:
            writeArmor(dos);
            break;
        case 1:
            writeContainer(dos);
            break;
        case 2:
            writeDrug(dos);
            break;
        case 3:
            writeWeapon(dos);
            break;
        case 4:
            writeAmmo(dos);
            break;
        case 5:
            writeMiscItem(dos);
            break;
        case 6:
            writeKey(dos);
            break;
        }

    }

    private void writeKey(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("unknown"));
    }

    private void writeMiscItem(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("powerPID"));
        dos.writeInt(m_fields.get("powerType"));
        dos.writeInt(m_fields.get("charges"));
    }

    private void writeAmmo(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("caliber"));
        dos.writeInt(m_fields.get("quantity"));
        dos.writeInt(m_fields.get("acAjust"));
        dos.writeInt(m_fields.get("drAjust"));
        dos.writeInt(m_fields.get("dmgMult"));
        dos.writeInt(m_fields.get("dmgDiv"));
    }

    private void writeWeapon(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("animCode"));
        dos.writeInt(m_fields.get("minDmg"));
        dos.writeInt(m_fields.get("maxDmg"));
        dos.writeInt(m_fields.get("dmgType"));
        dos.writeInt(m_fields.get("maxRange1"));
        dos.writeInt(m_fields.get("maxRange2"));
        dos.writeInt(m_fields.get("projFID"));
        dos.writeInt(m_fields.get("minST"));
        dos.writeInt(m_fields.get("apCost1"));
        dos.writeInt(m_fields.get("apCost2"));
        dos.writeInt(m_fields.get("critFail"));
        dos.writeInt(m_fields.get("perk"));
        dos.writeInt(m_fields.get("rounds"));
        dos.writeInt(m_fields.get("caliber"));
        dos.writeInt(m_fields.get("ammoPID"));
        dos.writeInt(m_fields.get("maxAmmo"));
        dos.writeByte(m_fields.get("shotSoundID"));
    }

    private void writeDrug(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("stat0"));
        dos.writeInt(m_fields.get("stat1"));
        dos.writeInt(m_fields.get("stat2"));

        dos.writeInt(m_fields.get("0amount0"));
        dos.writeInt(m_fields.get("0amount1"));
        dos.writeInt(m_fields.get("0amount2"));

        dos.writeInt(m_fields.get("1duration"));
        dos.writeInt(m_fields.get("1amount0"));
        dos.writeInt(m_fields.get("1amount1"));
        dos.writeInt(m_fields.get("1amount2"));

        dos.writeInt(m_fields.get("2duration"));
        dos.writeInt(m_fields.get("2amount0"));
        dos.writeInt(m_fields.get("2amount1"));
        dos.writeInt(m_fields.get("2amount2"));

        dos.writeInt(m_fields.get("addictionRate"));
        dos.writeInt(m_fields.get("wEffect"));
        dos.writeInt(m_fields.get("wOnSet"));
    }

    private void writeContainer(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("maxSize"));
        dos.writeInt(m_fields.get("containerFlags"));
    }

    private void writeArmor(DataOutputStream dos) throws IOException
    {
        dos.writeInt(m_fields.get("ac"));

        dos.writeInt(m_fields.get("DRNormal"));
        dos.writeInt(m_fields.get("DRLaser"));
        dos.writeInt(m_fields.get("DRFire"));
        dos.writeInt(m_fields.get("DRPlasma"));
        dos.writeInt(m_fields.get("DRElectrical"));
        dos.writeInt(m_fields.get("DREMP"));
        dos.writeInt(m_fields.get("DRExplode"));

        dos.writeInt(m_fields.get("DTNormal"));
        dos.writeInt(m_fields.get("DTLaser"));
        dos.writeInt(m_fields.get("DTFire"));
        dos.writeInt(m_fields.get("DTPlasma"));
        dos.writeInt(m_fields.get("DTElectrical"));
        dos.writeInt(m_fields.get("DTEMP"));
        dos.writeInt(m_fields.get("DTExplode"));

        dos.writeInt(m_fields.get("perk"));
        dos.writeInt(m_fields.get("maleFID"));
        dos.writeInt(m_fields.get("femaleFID"));
    }
}
