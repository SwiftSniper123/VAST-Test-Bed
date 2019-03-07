#include "pch.h"
//#include "../../Project1/Project1/VComponent.h"
#include "..\..\Project1\Project1\Enivornment.h"
//#include "../../Project1/Project1/Enivornment.h"
#include "test.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(TestCasename1, TestName1) {

	Vector3 info = { 0,0,0 }, info2 = { 0,0,0 }, info3 = { 0,0,0 };
	Obstacle guy(info, info2, info3);
	VComponent& vcomp = guy;
	
    EXPECT_EQ(typeid(guy), typeid(vcomp));
}

TEST(EnvironmentTest, EnvIsVComponentTest)
{
	Vector3 dim1 = { 0,0,0 };
	int num1 = 0;
	Environment ent(dim1, num1);
	VComponent& vcomp = ent;

	EXPECT_EQ(typeid(ent),typeid(vcomp));

}


TEST(EnvironmentTest, EnvConstructor1Test)

{

	// create an Env object and pass dummy parameters into it: vector3{0,0,0} and then 0 for //number of obstacles 

	Vector3 vecInput={0, 0, 0};

	Environment* env = new Environment(vecInput, 0); // anytime there is a ¡°new¡±, make sure there //is a delete somewhere at the end of life 

	// check first parameter sets the vector dimensions value 
	

	EXPECT_EQ(env->getDimensions(), vecInput);
	


	// check second parameter sets the number of obstacles  

	EXPECT_EQ(env->getNumObstacles(), 0);



	// cleanup 

	delete env;

}


//
//TEST(EnvironmentTest, EnvConstructor2Test)
//
//{
//
//	// involves the map, but you can¡¯t check it...however, you¡¯ll need it for the update function.   
//
//	// need to think about this***** 
//
//}



TEST(EnvironmentTest, EnvAddAndGetObstacleTest)

{

	// using a combined approach with the add and the get obstacle,
	//test that the obstacle YOU 
	//create can be added to the env object, 
	//and then retrieved as the SAME object. 


	Obstacle *obs = new Obstacle(Vector3{ 0,0,0 }, 
		Vector3{ 0,0,0 }, Vector3{ 0,0,0 });


	string name = "name";
	Environment* env = new Environment(Vector3{0,0,0}, 0);
	env->AddObstacle(name,obs);

	EXPECT_EQ(env->getObstacle(name), obs);

	delete obs, env;
}


TEST(ObsatcleTest, ObsIsVComponentTest)
{
	
	Obstacle obs(Vector3{ 0,0,0 },
		Vector3{ 0,0,0 }, Vector3{ 0,0,0 });
	VComponent& vcomp = obs;

	EXPECT_EQ(typeid(obs), typeid(vcomp));
}

TEST(ObsatcleTest, ObsConstructorTest)

{

	// create an Env object and pass dummy parameters into it: vector3{0,0,0}


	Vector3 vecpos = { 0, 0, 0 };
	Vector3 vecori = { 0, 0, 0 };
	Vector3 vecdim = { 0, 0, 0 };

	Obstacle* obs = new Obstacle(vecpos, vecori, vecdim);// anytime there is a ¡°new¡±, make sure there //is a delete somewhere at the end of life 

	// check first parameter sets the vector dimensions value 
	EXPECT_EQ(obs->getPosition(), vecpos);

	EXPECT_EQ(obs->getOrientation(), vecori);
	
	EXPECT_EQ(obs->getDimension(), vecdim);


	// cleanup 

	delete obs;

}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}