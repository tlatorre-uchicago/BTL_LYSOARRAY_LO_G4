#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{// constructor

    // Initialization of variables in the constructor and messenger definitions for later modification in between runs (i.e. define commands that can be changed in terminal)
    
    TARGET_YIELD= 124.;
    LYSO_SCALERESOLUTION=0.1;
    Vovcon=3.5;
    /*
    LYSO_L = 57./2.;    
    fMessenger = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger->DeclareProperty("LYSO_L", LYSO_L, "Length of LYSO crystal in mm");

    LYSO_thick=3./2.;
    fMessenger_thick = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_thick->DeclareProperty("LYSO_thick", LYSO_thick, "Section dimension for the LYSO in mm");

    LYSO_YIELD=40000.;  //yield, constant
    fMessenger_YIELD = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_YIELD->DeclareProperty("LYSO_YIELD", LYSO_YIELD, "Ammount of photons created per MeV deposited in the crystal");

    LYSO_SCALERESOLUTION=0.; // factor to vary width of yield distribution, constant
    fMessenger_SR = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_SR->DeclareProperty("LYSO_SCALERESOLUTION", LYSO_SCALERESOLUTION, " ");

    Vovcon=3.5; // is it working??
    fMessenger_vov = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_vov->DeclareProperty("Vov", Vovcon, "SiPM detection overvoltage");

    GLUE_L = 0.15+0.1*G4UniformRand();   GLUE_L=GLUE_L/2.;
    RESIN_L =0.3+0.4*G4UniformRand();   RESIN_L=RESIN_L/2.;


    fMessenger_ResinL = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_ResinL->DeclareProperty("GLUE_L", GLUE_L, "Glue thickness");


    fMessenger_GlueL = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_GlueL->DeclareProperty("RESIN_L", RESIN_L, "Resin thickness");

    XposTol = -0.05+G4UniformRand()*0.1;
    YposTol = -0.05+G4UniformRand()*0.1;
    fMessenger_XPos = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_XPos->DeclareProperty("XposTol", XposTol, "Glue average thickness");

    fMessenger_YPos = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_YPos->DeclareProperty("YposTol", YposTol, "Glue average thickness");

    GeomConfig=1;
    fMessenger_GC = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_GC->DeclareProperty("GeomConfig", GeomConfig, "1 == Bar ; 2 == Tile");
    
    perincr=0.;
    fMessenger_pi = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_pi->DeclareProperty("PerIncr", perincr, "LYSO section geometry parameter");

    ESRtrue=1;
    fMessenger_ESR = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_ESR->DeclareProperty("ESR", ESRtrue, "LYSO covered by ESR?");
    */
    
    
    // Run material function in the constructor
    DefineMaterial(); 

}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterial() // function to define a single time the materials when parametrizing, materials need to be defined in the class header! Definition of the function for the class::
{
/////////////
//Materials//
/////////////
    // import nist material data
    G4NistManager *nist = G4NistManager::Instance();

    // Find material in G4 database: Air
    worldMat = nist -> FindOrBuildMaterial("G4_AIR");
    G4double energyWorld[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};   // Calculate momentum from wavelength [0.2,0.9]nm to energy
    G4double rindexWorld[2] ={1.0, 1.0};                          // Define Refractive index for aerogel (constant, without dispersion in this case)
    //Refer material properties to material 
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energyWorld, rindexWorld, 2);    // AddProperty("Mat Prop name", energy, values, num points)
    worldMat->SetMaterialPropertiesTable(mptWorld);

    // Define material class: SiO2 -> G4Material (name, density, number of components)
    SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
    SiO2 -> AddElement(nist->FindOrBuildElement("Si"),1);
    SiO2 -> AddElement(nist->FindOrBuildElement("O"),2);
    G4MaterialPropertiesTable *mptSiO2 = new G4MaterialPropertiesTable();
    G4double energySiO2[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
    G4double rindexSiO2[2] ={1.4585,1.4585};  
    G4double ABSSiO2[2] ={0.01*mm,0.01*mm}; // absorption 
    mptSiO2->AddProperty("RINDEX", energySiO2, rindexSiO2, 2);    // AddProperty("Mat Prop name", energy, values, num points)
    mptSiO2->AddProperty("ABSLENGTH", energySiO2, ABSSiO2,2);
    SiO2->SetMaterialPropertiesTable(mptSiO2);
    
    // Define material class: WSi -> G4Material (name, density, number of components)
    WSi = new G4Material("WSi", 2.201*g/cm3, 2);
    WSi -> AddElement(nist->FindOrBuildElement("W"),1);
    WSi -> AddElement(nist->FindOrBuildElement("Si"),1);
    G4MaterialPropertiesTable *mptWSi = new G4MaterialPropertiesTable();
    G4double energyWSi[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
    // NEED TO FIND THIS INFORMATION
    G4double rindexWSi[2] ={1.4585,1.4585};  
    G4double ABSWSi[2] ={0.01*mm,0.01*mm}; // absorption 
    mptWSi->AddProperty("RINDEX", energyWSi, rindexWSi, 2);    // AddProperty("Mat Prop name", energy, values, num points)
    mptWSi->AddProperty("ABSLENGTH", energyWSi, ABSWSi,2);
    WSi->SetMaterialPropertiesTable(mptWSi);
 
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Target  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // make GaAs first
    G4double 	gaas_density = 5.32*g/cm3;  // from wikipedia
    prelude = new G4Material("GaAs", gaas_density, 2);
    // composition by fraction of mass
    prelude->AddElement(nist->FindOrBuildElement("Ga"),50.*perCent);
    prelude->AddElement(nist->FindOrBuildElement("As"),50.*perCent);
    
    G4double Sicont = 0.0000191154;
    G4double Bcont = 0.000286625;
    G4double target_density = 5.32023*g/cm3;
    scintillator = new G4Material("scintillator", target_density ,3);
    scintillator->AddMaterial(prelude,(100-(Sicont+Bcont)*100)*perCent);
    scintillator->AddElement(nist->FindOrBuildElement("Si"), (Sicont*100)*perCent);
    scintillator->AddElement(nist->FindOrBuildElement("B"), (Bcont*100)*perCent);

    const G4int num = 13;
    G4double targ_ene[num]   =  {1.3355*eV, 1.3455*eV, 1.3555*eV, 1.3655*eV, 1.3755*eV, 1.3485*eV, 1.3955*eV, 1.3455*eV, 1.4055*eV, 1.4155*eV, 1.4255*eV, 1.4355*eV, 1.4399*eV};
    
    // energy dependant refraction index
    G4double targ_r[num] =  {3.5455, 3.5503, 3.5556, 3.5606, 3.5662, 3.5722, 3.5781, 3.5838, 3.5903, 3.5973, 3.6047, 3.6074};
    
    // decay energy spectrum
    const G4int numB = 25;
    G4double targ_ene_B[numB] = {0.*eV, 900.9651*eV, 903.7816*eV, 906.6418*eV, 910.713*eV, 914.1872*eV, 918.2944*eV, 922.9947*eV, 927.714*eV, 932.4523*eV, 937.7951*eV, 942.556*eV, 947.3338*eV, 951.5281*eV, 955.7375*eV, 959.9411*eV, 964.1522*eV, 968.9398*eV, 973.1377*eV, 977.9308*eV, 982.7107*eV, 987.4792*eV, 992.2344*eV, 996.9859*eV, 0.*eV};
    G4double intensity_B[numB]  =  {0., 3.732,  6.2516,  8.0377,  9.4089,  10.8439,  11.6092,  12.3743,  12.8204,  12.9476,  12.8833,  12.6277,  12.085,  11.3829,  10.4255,  9.5638,  8.5747,  7.8725,  7.1066,  6.3087,  5.7341,  5.3509,  5.191,  5.0948, 0.};
    
    const G4int numC = 20;
    G4double targ_ene_C[numC] = {0.*eV, 998.1723*eV, 1008.2515*eV, 1018.3155*eV, 1028.37*eV, 1038.4226*eV, 1048.4639*eV, 1058.5203*eV, 1068.5824*eV, 1078.654*eV, 1088.7444*eV, 1098.2379*eV, 1108.3284*eV, 1118.4207*eV, 1128.5188*eV, 1138.6339*eV, 1148.7414*eV, 1158.2519*eV, 1168.3518*eV, 0.*eV};
    G4double intensity_C[numC]  =  {0., 5.0947,  5.1895,  5.5394,  6.0489,  6.5903,  7.323,  7.8005,  8.1824,  8.4048,  8.3082,  8.2753,  8.1788,  8.0503,  7.826,  7.3148,  6.9311,  6.6112,  6.3551, 0.};
    
    const G4int numD = 46;
    G4double targ_ene_D[numD] = {0.*eV, 1170.7266*eV, 1180.8114*eV, 1190.8885*eV, 1200.9715*eV, 1211.0431*eV, 1221.1071*eV, 1231.1597*eV, 1241.2029*eV, 1251.2423*eV, 1261.2892*eV, 1270.7448*eV, 1280.8126*eV, 1290.8898*eV, 1300.9482*eV, 1311.0084*eV, 1321.078*eV, 1331.1458*eV, 1341.2306*eV, 1350.7354*eV, 1360.8656*eV, 1370.9902*eV, 1381.1071*eV, 1391.226*eV, 1400.7327*eV, 1410.8364*eV, 1420.9496*eV, 1431.0647*eV, 1441.1911*eV, 1451.3043*eV, 1460.8186*eV, 1470.9261*eV, 1481.0298*eV, 1491.1373*eV, 1501.2373*eV, 1510.7383*eV, 1520.8401*eV, 1530.9401*eV, 1541.0457*eV, 1551.1418*eV, 1561.2342*eV, 1570.7315*eV, 1580.8237*eV, 1590.9085*eV, 1597.4417*eV, 0.*eV};
    G4double intensity_D[numD]  =  {0., 6.323, 6.3221, 6.4487, 6.4797, 6.7021, 7.0521, 7.5934, 8.2943, 9.0589, 9.6959, 10.3011, 10.5873, 10.714, 11.1596, 11.5734, 11.8277, 12.1138, 12.1129, 11.8887, 11.1223, 10.4515, 9.9083, 9.3332, 9.0772, 8.7572, 8.2779, 7.7666, 7.0639, 6.5845, 6.2009, 5.8172, 5.4973, 5.1136, 4.8575, 4.6971, 4.4091, 4.153, 3.8012, 3.6089, 3.4804, 3.3838, 3.2553, 3.2544, 3.1262, 0.};
    
    // TO CORRECT	
    // absorption length
    G4double target_absv2[2]   =  {1.*m, 1.*m};    
    //attenuation length, average distance traveled by photon before it is scattered
    G4double target_ene[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
    G4double target_scat[2] = {1.*m, 1.*m};
    
    
    G4MaterialPropertiesTable *mptScint= new G4MaterialPropertiesTable();
    
    //properties  
    mptScint->AddProperty("RINDEX", targ_ene, targ_r,num);
    mptScint->AddConstProperty("SCINTILLATIONYIELD", TARGET_YIELD / keV);//Word data check
    
    // TO CORRECT
    mptScint->AddProperty("ABSLENGTH", target_ene, target_absv2,2);
    mptScint->AddProperty("RAYLEIGH", target_ene, target_scat,2);
    mptScint->AddConstProperty("RESOLUTIONSCALE", LYSO_SCALERESOLUTION); //10%
    
    // Band B
    mptScint->AddProperty("SCINTILLATIONCOMPONENT1", targ_ene_B, intensity_B, numB);
    mptScint->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 250 * ns); // decay time constant
    mptScint->AddConstProperty("SCINTILLATIONYIELD1", 0.333);//the fraction of photons in each component must be specified, all to component 1
    mptScint->AddConstProperty("SCINTILLATIONRISETIME1", 8 * ns); // rise time
    
    // Band C
    mptScint->AddProperty("SCINTILLATIONCOMPONENT2", targ_ene_C, intensity_C, numC);
    mptScint->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 300 * ns); // decay time constant
    mptScint->AddConstProperty("SCINTILLATIONYIELD2", 0.283);
    mptScint->AddConstProperty("SCINTILLATIONRISETIME2", 8 * ns); // rise time
    
    // Band D
    mptScint->AddProperty("SCINTILLATIONCOMPONENT3", targ_ene_D, intensity_D, numD);
    mptScint->AddConstProperty("SCINTILLATIONTIMECONSTANT3", 1500 * ns); // decay time constant
    mptScint->AddConstProperty("SCINTILLATIONYIELD3", 0.384);
    mptScint->AddConstProperty("SCINTILLATIONRISETIME3", 8 * ns); // rise time
    
    // set it to scintillator material
    scintillator-> SetMaterialPropertiesTable(mptScint);    
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
////////////////////
// SOLID VOLUMES  // G4Box("var", width*m, length*m, thickness*m);
////////////////////

    G4double xWorld = 0.1*m;    xWorld=xWorld/2.;
    G4double yWorld = 0.1*m;    yWorld=yWorld/2.;
    G4double zWorld = 0.1*m;    zWorld=zWorld/2.;
    /*
    G4double DET_L = 0.3*mm;    DET_L=DET_L/2.;    
    G4double RESIN_W =6.5*mm;   RESIN_W=RESIN_W/2.;
    G4double RESIN_T =3.1*mm;RESIN_T=RESIN_T/2.;
    G4double FR4_L =0.8/2.*mm;     //FR4_L=FR4_L/2.;
    G4double DET_T =3.;DET_T=DET_T/2.;
    */

    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
    
    //TO DO: FIX THE DIMENSIONS BY DIVIDING BY TWO
    solidDetector = new G4Box("solidDetector", 1.*mm/2, 4.7*nm/2, 1.*mm/2);
    solidTarget = new G4Box("solidTarget", 5.*mm/2, 0.625*mm/2, 5.*mm/2);


////////////////////
// LOGIC VOLUMES  // G4LogicalVolume(solid_, material, "name");
////////////////////
    logicDetector = new G4LogicalVolume(solidDetector, SiO2, "locigDetector"); // Defined outside in class

    logicTarget = new G4LogicalVolume(solidTarget, scintillator, "logicTarget");

    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

//////////////////////
// PHYSICAL VOLUMES // G4PVPlacement(rotation,translation,logic_,"var",motherVolume,boolean,index if rep,check overlap);    
//////////////////////

    physWorld = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicWorld,"physWorld",0,false,0,true);
    physDetector = new G4PVPlacement(0,G4ThreeVector(0., -0.02*m-4.7*nm/2-0.625*mm/2, 0.),logicDetector,"physDetector",logicWorld,false,1,true); 
    physTarget = new G4PVPlacement(0, G4ThreeVector(0., -0.02*m, 0.), logicTarget, "physTarget", logicWorld, false, 2, true);


    ConstructSDandField();

//RETURN//
    return physWorld;
}
void MyDetectorConstruction::ConstructSDandField()
{
MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector", Vovcon);

logicDetector->SetSensitiveDetector(sensDet);

}

