#include "QemuConfiguration.h"
#include <string>

const std::set<std::string> QemuConfiguration::theDefaultCpuTypes = {
    "aarch64", "alpha", "arm", "cris", "i386", "lm32", "m68k", "microblaze", "microblazeel", "mips",
    "mips64", "mips64el", "mipsel", "moxie", "or32", "ppc", "ppc64", "ppc64le", "sh4", "sh4eb",
    "sparc", "sparc64", "tricore", "unicore32", "x86_64", "xtensa", "xtenseb" };

const std::set<std::string> QemuConfiguration::theDefaultNetworkAdapters = {
    "virtio", "i82551", "i82557b", "i82559er", "ne2k_pci", "ne2k_isa", "pcnet", "rtl8139", "e1000",
    "smc91c111", "lance", "mcf_fec" };

const std::set<std::string> QemuConfiguration::theDefaultVgaTypes = {
    "cirrus", "std", "vmware", "qxl", "tcx", "cg3", "virtio" };

QemuConfiguration::QemuConfiguration()
{

}

bool QemuConfiguration::saveConfiguration(std::string pathname)
{

    return false;
}

bool QemuConfiguration::loadConfiguration(std::string pathname)
{

    return false;
}

// Emulation setup options
void QemuConfiguration::setDriveA(std::string filename, bool qcow2Format)
{
    theDriveA = filename;
    theDriveAQcow2 = qcow2Format;
}

std::string QemuConfiguration::getDriveA() const
{
    return theDriveA;
}

bool QemuConfiguration::getDriveAQCow2() const
{
    return theDriveAQcow2;
}

void QemuConfiguration::setDriveB(std::string filename, bool qcow2Format)
{
    theDriveB = filename;
    theDriveBQcow2 = qcow2Format;
}

std::string QemuConfiguration::getDriveB() const
{
    return theDriveB;
}

bool QemuConfiguration::getDriveBQCow2() const
{
    return theDriveBQcow2;
}

void QemuConfiguration::setOpticalDrive(std::string filename)
{
    theOpticalDrive = filename;
}

std::string QemuConfiguration::getOpticalDrive() const
{
    return theOpticalDrive;
}

void QemuConfiguration::setProcessorType(std::string processorName)
{
    theCpuType = processorName;
}

std::set<std::string> QemuConfiguration::getQemuProcessorTypeList()
{
    return theDefaultCpuTypes;
}

std::string QemuConfiguration::getProcessorType() const
{
    return theCpuType;
}

void QemuConfiguration::setNetworkAdapterType(std::string networkAdapterName)
{
    theNetworkAdapter = networkAdapterName;
}

std::set<std::string> QemuConfiguration::getQemuNetworkAdapterTypeList()
{
    return theDefaultNetworkAdapters;
}

std::string QemuConfiguration::getNetworkAdapterType() const
{
    return theNetworkAdapter;
}

void QemuConfiguration::enableHumanInterfaceSocket(bool enable)
{
    theHumanInterfaceEnabled = enable;
}

bool QemuConfiguration::humanInterfaceSocketEnabled() const
{
    return theHumanInterfaceEnabled;
}

void QemuConfiguration::setOtherOptions(std::string otherOptions)
{
    theOtherOptions = otherOptions;
}

std::string QemuConfiguration::getOtherOptions() const
{
    return theOtherOptions;
}

void QemuConfiguration::setMemorySize(uint16_t numMegabytes)
{
    theMemoryMb = numMegabytes;
}

std::set<std::string> QemuConfiguration::getMemorySizes()
{
    std::set<std::string> retVal;
    int curMemorySize = 128;
    while(curMemorySize <= (8 * 1024))
    {
        retVal.insert(std::to_string(curMemorySize));
        curMemorySize *= 2;
    }

    return retVal;
}

uint16_t QemuConfiguration::getMemorySize() const
{
    return theMemoryMb;
}

void QemuConfiguration::setVgaType(std::string vgaType)
{
    theVideoAdapter = vgaType;
}

std::set<std::string> QemuConfiguration::getVgaTypes()
{
    return theDefaultVgaTypes;
}

std::string QemuConfiguration::getVgaType() const
{
    return theVideoAdapter;
}

int QemuConfiguration::getNumberOfPortsPerInstance() const
{
    //qDebug() << __PRETTY_FUNCTION__ << " not implemented yet";
    return -1;
}

void QemuConfiguration::setStartingPortNumber(uint16_t portNumber)
{
    theStartingPortNumber = portNumber;
}

uint16_t QemuConfiguration::getStartingPortNumber() const
{
    return theStartingPortNumber;
}

void QemuConfiguration::setNumberOfCpus(uint8_t numCpus)
{
    theNumberCpus = numCpus;
}

uint8_t QemuConfiguration::getNumberOfCpus() const
{
    return theNumberCpus;
}

bool QemuConfiguration::getCommandLine(std::string & commandName,
                                       std::vector<std::string> & args) const
{


    //theSystemCommandArgs.clear();
    bool success;
    success = buildCpuArgs(commandName, args);
    success = success && buildDriveArgs(args);
    success = success && buildNetworkArgs(args);
    success = success && buildQmpArgs(args);
    success = success && buildMonitorSocketArgs(args);
    success = success && buildOtherArgs(args);
    success = success && buildMemoryArgs(args);

    if (!success)
    {
        //qWarning() << "Failed to build the command";
        return false;
    }

    //qDebug() << "Command:" << theSystemCommand;
    //qDebug() << "Args   :" << theSystemCommandArgs.join(" ");
    return success;
}

bool QemuConfiguration::buildCpuArgs(std::string & commandName, std::vector<std::string> & args) const
{
    commandName = "qemu-system-";

    if (theCpuType.empty())
    {
        //qWarning() << "Must specify a CPU type before starting emulator";
        return false;
    }

    commandName += theCpuType;

    if (theNumberCpus == 0)
    {
        //qWarning() << "Must specify a at least 1 CPU core";
        return false;
    }

    if (theNumberCpus != 1)
    {
        args.push_back("-smp");
        args.push_back(std::to_string(theNumberCpus));
    }

    return true;

}

bool QemuConfiguration::buildDriveArgs(std::vector<std::string> & args) const
{
    if (theDriveA.empty() && !theDriveB.empty() )
    {
        //qWarning() << "No drive A file specified, but drive B file specified";
        return false;
    }

    if (!theDriveA.empty())
    {
        args.push_back("-drive");

        std::string filearg = "file=";
        filearg += theDriveA;

        if (theDriveAQcow2)
        {
            filearg += ",format=qcow2";
        }
        args.push_back(filearg);
    }

    if (!theDriveB.empty())
    {
        args.push_back("-drive");

        std::string filearg = "file=";
        filearg += theDriveB;

        if (theDriveBQcow2)
        {
            filearg += ",format=qcow2";
        }
        args.push_back(filearg);
    }

    return true;
}

bool QemuConfiguration::buildNetworkArgs(std::vector<std::string> & args) const
{
    args.push_back("-net");
    args.push_back("nic,model=ne2k_pci,name=testNet");
    args.push_back("-net");
    args.push_back("user,id=testNet,hostfwd=tcp:127.0.0.1:2222-:23");
    return true;

}

bool QemuConfiguration::buildQmpArgs(std::vector<std::string> & args) const
{
    args.push_back("-qmp");

    std::string qmpArg = "tcp::";
    qmpArg += std::to_string(theStartingPortNumber);
    qmpArg += ",server,nowait";
    args.push_back(qmpArg);

    return true;
}

bool QemuConfiguration::buildMonitorSocketArgs(std::vector<std::string> & args) const
{
    if (theHumanInterfaceEnabled)
    {
        args.push_back("-monitor");
        std::string devCfg = "tcp::";
        devCfg += std::to_string(theStartingPortNumber+1);
        devCfg += ",server,nowait";
        args.push_back(devCfg);
    }

    return true;
}

bool QemuConfiguration::buildOtherArgs(std::vector<std::string> & args) const
{


    args.push_back("-display");
    args.push_back("sdl");
    return true;
}

bool QemuConfiguration::buildMemoryArgs(std::vector<std::string> & args) const
{
    args.push_back("-m");
    args.push_back(std::to_string(theMemoryMb));
    return true;
}