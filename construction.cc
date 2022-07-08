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
    G4double targ_ene_B[numB] = {0.*eV, 1.376126538455266*eV, 1.3718380461961193*eV, 1.3675102828173182*eV, 1.361397042023121*eV, 1.3562233034240718*eV, 1.3501574052199408*eV, 1.343281802519562*eV, 1.3364485006499873*eV, 1.3296572750498898*eV, 1.322081960475164*eV, 1.3154040548593424*eV, 1.308769922842405*eV, 1.3030009143524006*eV, 1.2972620456265476*eV, 1.2915813109075156*eV, 1.285940108140605*eV, 1.2795861872244307*eV, 1.274066336482496*eV, 1.2678217971373869*eV, 1.2616551181665188*eV, 1.2555626329466003*eV, 1.249545454513573*eV, 1.2435902898245625*eV, 0.*eV};
    G4double intensity_B[numB]  =  {0., 0.78017353, 1.30689518, 1.68027887, 1.96692784, 2.26691418, 2.42689992, 2.58684386, 2.68010093, 2.70669206, 2.69325016, 2.63981705, 2.52636577, 2.37959197, 2.17944778, 1.99930964, 1.79253857, 1.64574386, 1.48563269, 1.31883192, 1.19871196, 1.11860411, 1.08517706, 1.06506647, 0.};
    
    const G4int numC = 20;
    G4double targ_ene_C[numC] = {0.*eV, 1.2421121927867589*eV, 1.22969515476248*eV, 1.2175420921433509*eV, 1.205638033326529*eV, 1.193966680166632*eV, 1.1825318776659859*eV, 1.171297314120478*eV, 1.1602680189492196*eV, 1.1494343731465348*eV, 1.1387815031076187*eV, 1.1289375319609734*eV, 1.1186594012496678*eV, 1.10856494727968*eV, 1.0986453963655742*eV, 1.0888855358443152*eV, 1.0793046932338317*eV, 1.0704424351317727*eV, 1.06118891958056*eV, 0.*eV};
    G4double intensity_C[numC]  =  {0., 1.0650455697814645, 1.0848634825173042, 1.1580099768872445, 1.264520805356763, 1.3777003196519493, 1.5308710439299007, 1.6306922816025113, 1.7105283667693594, 1.7570210228078695, 1.7368268205897035, 1.7299490850516326, 1.7097757878047073, 1.6829128997608738, 1.6360230492687973, 1.5291568362881929, 1.4489444616390186, 1.3820694586411797, 1.328531827294676, 0.};
    
    const G4int numD = 46;
    G4double targ_ene_D[numD] = {0.*eV, 1.0590363149961763*eV, 1.0499915433844917*eV, 1.0411066899478856*eV, 1.0323658674098446*eV, 1.0237802307217656*eV, 1.015342539841102*eV, 1.0070521186910215*eV, 0.9989035510084632*eV, 0.9908888025380875*eV, 0.9829957985305847*eV, 0.9756813361203623*eV, 0.9680120138824387*eV, 0.96045532649805*eV, 0.9530294782928347*eV, 0.9457162778911276*eV, 0.9385077825321461*eV, 0.9314096054181311*eV, 0.924406276095999*eV, 0.9179014515589082*eV, 0.9110686494919132*eV, 0.9043405155864735*eV, 0.8977160310970833*eV, 0.891186611184669*eV, 0.8851381739942262*eV, 0.8787992600219292*eV, 0.8725446591012114*eV, 0.8663773093781172*eV, 0.8602897869213891*eV, 0.8542949844026524*eV, 0.8487309679189479*eV, 0.8428988950104308*eV, 0.8371485734669231*eV, 0.8314740596536633*eV, 0.8258800819377473*eV, 0.8206861402348788*eV, 0.8152349378031276*eV, 0.809856626220714*eV, 0.8045458900615358*eV, 0.799309247118479*eV, 0.7941422141098384*eV, 0.7893404979348808*eV, 0.7843012375965787*eV, 0.779329536759658*eV, 0.7761422431454008*eV, 0.*eV};
    G4double intensity_D[numD]  =  {0., 1.3218213315265277, 1.3216331867853646, 1.3480988803756315, 1.354579421460136, 1.4010720774986463, 1.4742394768398268, 1.5873980861637726, 1.7339210295872969, 1.8937604396909158, 2.0269251064918645, 2.1534419924383856, 2.2132720201282314, 2.2397586186897387, 2.3329111705366814, 2.419415941529197, 2.4725772833933752, 2.53238640611198, 2.532198261370817, 2.485329315849981, 2.3251136162640353, 2.184883069183853, 2.071327265406341, 1.951102775803161, 1.8975860494278975, 1.830690141458818, 1.7304926143038817, 1.623605496352037, 1.4767062634461867, 1.3764878313200097, 1.2962963616420757, 1.2160839869929014, 1.1492089839950625, 1.0689966093458882, 1.0154589779993843, 0.9819274041298833, 0.9217210869577119, 0.868183455611208, 0.7946397667877015, 0.7544395070925329, 0.7275766190486994, 0.7073824168305336, 0.6805195287867002, 0.6803313840455372, 0.6535312109154247, 0.};
    
    // TO CORRECT	
    // absorption length
    G4double target_absene[18] = {1.5819*eV, 1.5872*eV, 1.592*eV, 1.597*eV, 1.6019*eV, 1.6073*eV, 1.6122*eV, 1.6171*eV, 1.6219*eV, 1.627*eV, 1.6319*eV, 1.6372*eV, 1.6421*eV, 1.6471*eV, 1.652*eV, 1.657*eV, 1.6619*eV, 1.6665*eV};
    G4double target_absv2[18]   =  {0.91327*cm, 0.38512*cm, 0.1762*cm, 0.081546*cm, 0.038194*cm, 0.018746*cm, 0.009196*cm, 0.0047841*cm, 0.0027638*cm, 0.001489*cm, 0.00082111*cm, 0.00045814*cm, 0.00028392*cm, 0.00017803*cm, 0.00011426*cm, 0.000079579*cm, 0.00006378*cm, 0.00005809*cm};    
    //attenuation length, average distance traveled by photon before it is scattered
    G4double target_ene[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
    G4double target_scat[2] = {10.*m, 10.*m};
    
    
    G4MaterialPropertiesTable *mptScint= new G4MaterialPropertiesTable();
    
    //properties  
    mptScint->AddProperty("RINDEX", targ_ene, targ_r,num);
    mptScint->AddConstProperty("SCINTILLATIONYIELD", TARGET_YIELD / keV);//Word data check
    
    // TO CORRECT
    mptScint->AddProperty("ABSLENGTH", target_absene, target_absv2,2);
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

