
#include "ExtendibleHashing.h"
#include <bitset>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;


void displayNotFound(int key) {
	std::cout << "\t\t\t key:  " << key << " \t value : \t not found" << std::endl;
}

//Display one record entry, please don't change this function
void displayItem(DataItem* dataItem) {
	if (dataItem != 0 && dataItem->valid)
	{
		std::cout << "\t\t\t key: " << std::bitset<8>(dataItem->key) << "\t value:\t" << dataItem->data << std::endl;
	}
	else {
		std::cout << "\t\t\t key:  ============ \t value:\t empty data" << std::endl;
	}
}

void displayBucket(Bucket& currentBucket, string& depths, string& values, int verbose) {
	depths.append(std::to_string(currentBucket.localDepth));
	depths.append(",");
	values.append("[");
	if (verbose)
		std::cout << "\t\tBucket:\t local depth:" << currentBucket.localDepth << std::endl;
	if (currentBucket.currentEntries == 0)
	{
		if (verbose)
			std::cout << "\t\t\tNo Data yet\n";
		for (int i = 0; i < RECORDSPERBUCKET; i++)
		{
			values.append("null");
			values.append(",");
		}
	}
	else {
		for (int i = 0; i < RECORDSPERBUCKET; i++)
		{
			if (currentBucket.dataItem[i].valid)
			{
				values.append(std::to_string(currentBucket.dataItem[i].data));
				values.append(",");
				if (verbose)
					std::cout << "\t\t\t key: " << std::bitset<8>(currentBucket.dataItem[i].key) << "\t value:\t" << currentBucket.dataItem[i].data << std::endl;
			}
			else {
				values.append("null");
				values.append(",");
				if (verbose)
					std::cout << "\t\t\t key:  ============ \t value:\t empty data" << std::endl;
			}

		}
	}
	values.pop_back();
	values.append("]");

}

void displayDirectory(GlobalDirectory& globaldirectory, Bucket& currentBucket, int verbose) {
	std::cout << "Directory:\t global depth:" << globaldirectory.globalDepth << std::endl;
	string values = "(";
	string depths = "(";
	int count = 0;
	//string locations = "(";
	if (globaldirectory.length == 0)
	{
		count++;
		std::cout << "\tNo Directory yet\n";
		displayBucket(currentBucket, depths, values, verbose);
	}
	else {

		for (int i = 0; i < globaldirectory.length; i++)
		{
			if (i == 0)
			{
				count++;
			}
			else {
				if (globaldirectory.entry[i - 1] != globaldirectory.entry[i])
					count++;
			}
			if (verbose)
				std::cout << "\t key: " << std::bitset<8>(i) << "\t value:\t" << globaldirectory.entry[i] << std::endl;
			displayBucket(*globaldirectory.entry[i], depths, values, verbose);
			if (verbose)
				std::cout << "-----------------------------------------------\n\n";
		}
		//values.pop_back();
		depths.pop_back();

	}

	values.append(")");
	depths.append(")");
	std::cout << " buckets:\t" << count << "/" << globaldirectory.length << endl;
	std::cout << "values:\t" << values << endl;
	std::cout << "depths:\t" << depths << endl;
	std::cout << "=========================\n";
	//std::cout << "Press any key to continue\n";
	char t[100];
	//std::cin >> t;


}



//Hashing function and getting directory Index, please don't change this function
int getCurrentHash(int key, int depth) {
	int hashedKey = (key & MAXKEYVALUE) >> (MAXKEYLENGTH - depth);
	return hashedKey;
}

//TODO1: Implement this function, Don't change the interface please
// functionlity: try to insert item into a bucket
// return:   1 if succedded
//			 0 if failed
// input:   currentBucket, dataItem to be inserted (check ExtendiableHashing.h for the content of the file)
// Hint1: don't forget to update currentEntries upon insertion, you will need it later

int insertItemIntoBucket(Bucket& currentBucket, DataItem data)
{
	//getting the key 
	//int key = data.key;
	//hashing the key to get h(k) using the local depth of this bucket
	//int hk = getCurrentHash(key, currentBucket.localDepth);
	//checking if there is enough space in the bucket
	if (currentBucket.currentEntries < RECORDSPERBUCKET) // to know this var i had to check the code
	{
		//currentBucket.dataItem[currentBucket.currentEntries] = data;
		//currentBucket.currentEntries++;
		//return 1;
		// i will loop on the data, checking the first non valid and replacing it
		for (int i = 0; i < RECORDSPERBUCKET; i++)
		{
			if (currentBucket.dataItem[i].valid == 0)
			{
				currentBucket.dataItem[i] = data;
				currentBucket.currentEntries++;
				return 1;
			}
		}


	}
	//if the bucket is full 0 will be returned
	return 0;
}

//TODO2: Implement this function, Don't change the interface please
// functionlity: look for an item in a bucket using key, if found call displayItem(..), if not found call displayNotFound()
// return:   nothing
// input:   currentBucket, key value to search for
// Hint: use displayNotFound & displayItem functions

void findItemInBucket(Bucket& currentBucket, int key)
{
	// i will need to check the whole bucket using a for loop, i will check
	// non empty spaces only so i will loop recordsperbucket times
	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{
		//checking if the key is found, and it is a valid entry
		if (currentBucket.dataItem[i].key == key && currentBucket.dataItem[i].valid == 1)
		{
			//if found i will display the item
			displayItem(&currentBucket.dataItem[i]);
			return;
		}
	}
	//displaying not found
	displayNotFound(key);
}


//TODO3: Implement this function, Don't change the interface please
// functionlity: try to Delete item based on a key value from a bucket
// return:   1 if succedded
//			 0 if failed (when does it fail to delete??)
// input:   currentBucket, key to be inserted (check ExtendiableHashing.h)
// Hint1:   don't forget to update currentEntries upon deletion, you will need it later

//upon checking, i believe it fails if it does not find a valid entry with the key
int deleteItemFromBucket(Bucket& currentBucket, int key)
{
	// i will need to check the whole bucket using a for loop
	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{
		//checking if the key is found
		if (currentBucket.dataItem[i].key == key)
		{
			if (currentBucket.dataItem[i].valid == 0)
			{
				//if the item is not valid, i will return 0, failed to delete
				return 0;
			}
			//if found i will delete the item
			currentBucket.dataItem[i].valid = 0;
			//decrementing the currentEntries
			currentBucket.currentEntries--;
			return 1;
		}
	}
	return 0;
}


//TODO4: Implement this function, Don't change the interface please
// functionlity: try to insert item in the file, if the bucket is full, extend the directory,
//				  if you extended the directory five times but it still doesn't work, return 0 
// return:   1 if succedded
//			 0 if failed (when does it fail to insert??)
// input:   data:--> dataitem to be inserted, currentBucket --> initialBucket before creating director , globaldirectory
// Hint: use insertItemIntoBucket,getCurrentHash, extendDirectory functions
// Hint1:   don't forget to check for corner cases, for example if several entries points to the same bucket and you are going to split it
// Hint2:   a bucket could be split without expanding the directory (think when this will happen?)
// Hint3:   don't forget to delete extra data

void splitCell(int key, int hk, GlobalDirectory& globaldirectory)
{
	//the current hash, old bucket will be the same but with zero to its right
			// i need to get the hash using the local depth first
	int newHk = getCurrentHash(key, (globaldirectory.entry[hk]->localDepth ));
	int firstNbits = newHk << 1; // 0 to 00
	// the new hash = old hash + 1
	int firstNbitsPlusOne = firstNbits + 1;

	// now i want to re assign the pointers of the directory
	// now to know how many buckets for each index, i deduced the formula 2^(globaldepth - newlocaldepth)
	int amountForEach = 1<< (globaldirectory.globalDepth - (globaldirectory.entry[hk]->localDepth + 1));


	Bucket* emptyBucket1 = new Bucket(globaldirectory.entry[hk]->localDepth + 1);
	Bucket* emptyBucket2 = new Bucket(globaldirectory.entry[hk]->localDepth + 1);
	// the original old bucket copy
	Bucket oldBucketCopy = *globaldirectory.entry[hk];
	// i will loop on the whole directory, all entries starting with the same start as the 
	//new bucket will be assigned to it
	// the first number in this pattern will be hk shifted left by global - local times
	for (auto item : (oldBucketCopy).dataItem)
	{
		deleteItemFromBucket(*globaldirectory.entry[hk], item.key);
	}
	int start1 = firstNbits << (globaldirectory.globalDepth - (globaldirectory.entry[hk]->localDepth + 1));
	int start2 = firstNbitsPlusOne << (globaldirectory.globalDepth - (globaldirectory.entry[hk]->localDepth + 1));
	for (int i = start1; i < start1 + amountForEach; i++)
	{
		globaldirectory.entry[i] = emptyBucket1;

	}
	
	for (int i = start2; i < start2 + amountForEach; i++)
	{
		globaldirectory.entry[i] = emptyBucket2;
	}
	// now new bucket is good, i need to adjust the old bucket, this will be done by
	// getting its elements and reinserting + increasing its local depth
	// adjust depth of the cleaner old bucket
	//globaldirectory.entry[hk]->localDepth++;

	// clean the cleaner old bucket
	/*for(auto  item: (*oldBucket).dataItem )
	{
		deleteItemFromBucket(*globaldirectory.entry[hk] , item.key);
	}*/
	//now reinsert all items
	//loop on the data in oldbucket, call insert item on them
	for (auto item : (oldBucketCopy).dataItem)
	{
		//the bucket here is dummy since the function will call insert item into bucket
		//try using insert item into bucket
		insertItem(item, *globaldirectory.entry[hk], globaldirectory);
	}
}

int insertItem(DataItem data, Bucket& currentBucket, GlobalDirectory& globaldirectory)
{
	//First corner case is done for you: if no directory yet
	if (globaldirectory.globalDepth == 0) {
		if (insertItemIntoBucket(currentBucket, data) == 0)
		{
			createFirstTimeDirectory(globaldirectory, currentBucket);
		}
		else {
			return 1; //successfully inserted; //howww ???
		}
	}
	// complete the rest of the function ....
	//getting the key
	int key = data.key;
	//hashing the key to get h(k) using the global depth of the directory
	int hk = getCurrentHash(key, globaldirectory.globalDepth);

	//The * operator dereferences the pointer to get the actual Bucket object that globaldirectory.entry[hk] points to.
	if (insertItemIntoBucket(*globaldirectory.entry[hk], data) == 1)
	{
		return 1;
	}
	else //if the bucket is full, i will extend the directory
	{
		//check if i will simply split the bucket or extend the directory + split the bucket
		if (globaldirectory.entry[hk]->localDepth == globaldirectory.globalDepth)
		{
			//if the local depth of the bucket is equal to the global depth, i will extend the directory
			for (int i = 0; i < 5; i++)
			{
				//if the extension is successful, i will insert the item into the bucket
				// i will try max five times as said in the comment
				if (extendDirectory(globaldirectory, hk, key) == 1)
				{
					

					if (insertItem(data,currentBucket,globaldirectory) == 1)
					{
						return 1;
					}
				}
			}
			return 0;

		}
		else // now i need to split the cell
		{
			splitCell(key, hk, globaldirectory);
			//insert the item with hk to its new bucket now
			//insertItem(data, *globaldirectory.entry[hk], globaldirectory);
			return insertItem(data, currentBucket, globaldirectory); // insert the item in the new bucket


		}
	}

}

//TODO5: Implement this function, Don't change the interface please
// functionlity: search the directory for an item using the key
// return:   nothing
// input:   key to be searched for , currentBucket --> initialBucket before creating directory,  globaldirectory
// Hint1:   use findItemInBucket & getCurrentHash functions

void searchItem(int key, Bucket& currentBucket, GlobalDirectory& globaldirectory)
{
	//if the directory is empty, i will search in the bucket
	if (globaldirectory.globalDepth == 0)
	{
		findItemInBucket(currentBucket, key);
	}
	else
	{
		//getting the key
		int hk = getCurrentHash(key, globaldirectory.globalDepth);
		//searching in the bucket by calling the function
		findItemInBucket(*globaldirectory.entry[hk], key);
	}
}

//TODO6: Implement this function, Don't change the interface please
// functionlity: search on an item based on the key and delete it.
// return:   1 if succedded
//			 0 if failed (when does it fail to delete??)
// input:   key to be searched for , currentBucket --> initialBucket before creating directory,  globaldirectory
// Hint:    use deleteItemFromBucket & getCurrentHash & checkDirectoryMinimization functions
// Hint1:   in case the whole bucket is empty, the bucket should be merged again and the pointer should point to the peer bucket
// Hint2:   in how many steps do we reach our goal?
// Hint3:   in case of delete success don't forget to call checkDirectoryMinimization to compress directory if needed.
// Hint4: You might want to loop on checkDirectoryMinimization, not just call it once to continue merging


void tryMerge(int hk, GlobalDirectory& globaldirectory, int root1,int key)
{
	//check lower at start
	if (root1 % 2 != 0)
	{
		root1--;
	}
	Bucket oldBucket1 = *globaldirectory.entry[root1];
	int root2;
	if (((root1<<((globaldirectory.globalDepth) - (globaldirectory.entry[hk]->localDepth))) & (1 <<((globaldirectory.globalDepth)))) == 1)
	{
		root2 = root1 - (1<<((globaldirectory.globalDepth) - (globaldirectory.entry[hk]->localDepth)));
	}
	else
	{
		root2 = root1 + (1 << ((globaldirectory.globalDepth) - (globaldirectory.entry[hk]->localDepth)));
	}

	//now i will check merging conditions
	if (globaldirectory.entry[root1]->localDepth == globaldirectory.entry[root2]->localDepth &&
		((globaldirectory.entry[root1]->currentEntries) + (globaldirectory.entry[root2]->currentEntries)) <= RECORDSPERBUCKET)
	{
		// now i need to check if the total count of the valid items in both buckets <= RECORDSPERBUCKET

		//if the local depth of the two buckets is the same, i will merge them
		// i will loop on the data in the old bucket and insert them into the new bucket
		Bucket oldBucket2 = *globaldirectory.entry[root2];
		Bucket* newBucket = new Bucket(globaldirectory.entry[root1]->localDepth - 1);
		int newLocalDepth = globaldirectory.entry[root1]->localDepth - 1;
		int amountForEach = pow(2, globaldirectory.globalDepth - globaldirectory.entry[hk]->localDepth);
		int start1 = root1; //<< (globaldirectory.globalDepth - (globaldirectory.entry[hk]->localDepth));
			int start2 = root2;//<< (globaldirectory.globalDepth - (globaldirectory.entry[hk]->localDepth));
		for (int i = start1; i < start1 + amountForEach; i++)
		{
			globaldirectory.entry[i] = newBucket;

		}

		for (int i = start2; i < start2 + amountForEach; i++)
		{
			globaldirectory.entry[i] = newBucket;
		}
		for (auto item : (oldBucket1).dataItem)
		{
			if (item.valid == 1) //since we just deleted, we need to check for validity
			{
				insertItem(item, *newBucket, globaldirectory);
			}
		}
		for (auto item : (oldBucket2).dataItem)
		{
			if (item.valid == 1)
			{
				insertItem(item, *newBucket, globaldirectory);
			}
		}
		
		//deleting the old bucket
		//delete globaldirectory.entry[root2];
		//assigning the pointer to the new bucket
		//globaldirectory.entry[root2] = globaldirectory.entry[root1];
		//checking if the directory needs to be minimized
		int lowerRoot;
		if (root1 < root2)
		{
			lowerRoot = root1;
		}
		else
		{
			lowerRoot = root2;
		}
		int root1 = hk << (globaldirectory.globalDepth - oldBucket1.localDepth);
		checkDirectoryMinimization(globaldirectory);
		int newHash = getCurrentHash(key, globaldirectory.globalDepth);
		hk = newHash;
		lowerRoot =  newHash << (globaldirectory.globalDepth - newLocalDepth); //left shift 3shan awsl l awlha
		
		tryMerge(hk, globaldirectory, lowerRoot,key);
		


	}
	else
	{
		//did not merge
		return;
	}
}
int deleteItem(int key, Bucket& currentBucket, GlobalDirectory& globaldirectory)
{
	//if the directory is empty, i will search in the bucket
	if (globaldirectory.globalDepth == 0)
	{
		//deleting the item from the bucket
		if (deleteItemFromBucket(currentBucket, key) == 1)
		{
			//checking if the directory needs to be minimized
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		//getting the key
		int hk = getCurrentHash(key, globaldirectory.globalDepth);
		//deleting the item from the bucket
		if (deleteItemFromBucket(*globaldirectory.entry[hk], key) == 1)
		{
			//checking if the directory needs to be minimized
			// copy the original

			//trying to merge
			//first we will get the positions of merged roots
			Bucket oldBucket1 = *globaldirectory.entry[hk];
			int root1 = hk << (globaldirectory.globalDepth - oldBucket1.localDepth);

			tryMerge(hk, globaldirectory, root1,key);
			//checkDirectoryMinimization(globaldirectory);
			//deleting done, either merged or not
			return 1;
		}
		else //did not delete
		{
			return 0;
		}
	}
	return 0;

}





//create  the first directory, this might help you to implement extendDirectory
int createFirstTimeDirectory(GlobalDirectory& globaldirectory, Bucket& currentBucket)
{
	globaldirectory.globalDepth = 1;
	globaldirectory.length = 2;
	globaldirectory.entry = new Bucket * [globaldirectory.length];
	globaldirectory.entry[0] = new Bucket(globaldirectory.globalDepth);
	globaldirectory.entry[1] = new Bucket(globaldirectory.globalDepth);
	for (int i = 0; i < RECORDSPERBUCKET; i++) {
		int newKey = getCurrentHash(currentBucket.dataItem[i].key, globaldirectory.globalDepth);
		if (newKey == -1)
		{
			return -1;
		}
		insertItemIntoBucket(*globaldirectory.entry[newKey], currentBucket.dataItem[i]);
	}
	return 1;
}


//TODO7: Implement this function, Don't change the interface please
// functionlity: this expands the directory because we can't find a space anymore in the file,
//               it also redistrubtes data over the split buckets
// return:   1 if succedded
//			 0 if failed (when does it fail to expand??)
// input:   Directory, hashKey(bucket index) at which the overflow occurs
// Hint1:   don't forget todelete unneeded pointers to avoid memory leakage
// Hint2:   what is the size of the new directory compared to old one? what is the new depth?
// Hint3:   some entries will point to the same bucket
int extendDirectory(GlobalDirectory& globaldirectory, int splitIndex, int key)
{
	//check the edge case of having depth = maxlength
	if (globaldirectory.globalDepth == MAXKEYLENGTH)
	{
		return 0;
	}
	//get a  copy of the old directory entries
	Bucket** oldEntry = globaldirectory.entry;
	//update the global depth
	globaldirectory.globalDepth++;
	//storing old directory length
	int oldLength = globaldirectory.length;
	//update the length
	globaldirectory.length *= 2;
	//allocate new memory for the new directory
	globaldirectory.entry = new Bucket * [globaldirectory.length];
	int j = 0;
	for (int i = 0; i < oldLength; i++)
	{
		for (int k = 0; k < 2; k++)
		{
			globaldirectory.entry[j] = oldEntry[i];
			j++;
		}
	}
	splitCell(key, splitIndex, globaldirectory);
	return 1;
}



//If all buckets have depth less than global depth,
// compress the directory depth by one 
// loop on this function as appropriate to compress the file back
int checkDirectoryMinimization(GlobalDirectory& globaldirectory) {

	for (int i = 0; i < globaldirectory.length; i++) {
		if (globaldirectory.entry[i]->localDepth == globaldirectory.globalDepth)
			return -1;

	}
	//if its depth =1, there is nothing more we can do, stop here
	if (globaldirectory.length == 1)
		return -1;
	//all are smaller than localdepth, initiate minimization
	// this is actually too stupid, it should be at least half empty to initiate minimization
	// but we will keep it like that for simplicity
	int oldGlobalDepth, oldLength;
	Bucket** oldEntry;

	oldGlobalDepth = globaldirectory.globalDepth;
	oldLength = globaldirectory.length;
	globaldirectory.globalDepth -= 1;
	globaldirectory.length /= 2;
	oldEntry = globaldirectory.entry;
	globaldirectory.entry = new Bucket * [globaldirectory.length];
	for (int i = 0; i < globaldirectory.length; i++) {
		globaldirectory.entry[i] = oldEntry[2 * i];
	}

	delete[] oldEntry;
	//delete oldEntry;
	return 1;

}