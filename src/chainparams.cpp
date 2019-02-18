// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2018 The PIVX developers
// Copyright (c) 2018 The Midas developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x00000d1c54783a9a44565a4c240f7136bea4ddb55f08ff8411482c28b3bafa95"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1538648113, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    100        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1538648113,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1538648113,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xc3;
        pchMessageStart[1] = 0x4a;
        pchMessageStart[2] = 0x5f;
        pchMessageStart[3] = 0xde;
        vAlertPubKey = ParseHex("046c83c10a2137b7ed555c663aeac44628923bf53bed6fa2c26d86ed1323ded1794c37c2cae1b069d40e3258f31170dae0b2924bc38412194645ee4bd64b433358");
        nDefaultPort = 44433;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // Midas starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 105000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 24 * 60 * 60; // Midas: 1 day
        nTargetSpacing = 2 * 60;  // Midas: 2 minute // test
        nMaturity = 20;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 5000000 * COIN;
        nStakeInputMinimal = 20 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 300;
        nModifierUpdateBlock = 615800;
        nZerocoinStartHeight = 30000;
        nZerocoinStartTime = 1508214600; // October 17, 2017 4:30:00 AM
        nBlockEnforceSerialRange = 895400; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 891730; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 902850; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 268200*COIN; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 1153160; //!> The block that zerocoin v2 becomes active - roughly Tuesday, May 8, 2018 4:00:00 AM GMT
        nEnforceNewSporkKey = 1525158000; //!> Sporks signed after (GMT): Tuesday, May 1, 2018 7:00:00 AM GMT must use the new spork key
        nRejectOldSporkKey = 1527811200; //!> Fully reject old spork key after (GMT): Friday, June 1, 2018 12:00:00 AM

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "Midas coin created at 11/08/2017 by HaZZarDt";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("045e5ca9781c965c343b76bc15fdc2a7a4e953decdeff6f2a18be87d1920e450c02bd92f0a3ebd6f4fea4b116a7186ce4aa2f95516974b5b43c19b4add4a4a4da1") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1538648113;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 888019;

        hashGenesisBlock = genesis.GetHash();
        //printf("hashGenesisBlock = %s\n",hashGenesisBlock.ToString().c_str());
		//printf("hashMerkleRoot = %s\n",genesis.hashMerkleRoot.ToString().c_str());

        assert(hashGenesisBlock == uint256("0x00000d1c54783a9a44565a4c240f7136bea4ddb55f08ff8411482c28b3bafa95"));
        assert(genesis.hashMerkleRoot == uint256("0x08848e3b727dc280f5542aa778a7042ccf58093595f3030d82c5de73df4ccff8"));

        /* vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "midas.seed.fuzzbawls.pw"));     // Primary DNS Seeder from Fuzzbawls
        vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "midas.seed2.fuzzbawls.pw"));    // Secondary DNS Seeder from Fuzzbawls
        vSeeds.push_back(CDNSSeedData("coin-server.com", "coin-server.com"));         // Single node address
        vSeeds.push_back(CDNSSeedData("s3v3nh4cks.ddns.net", "s3v3nh4cks.ddns.net")); // Single node address
        vSeeds.push_back(CDNSSeedData("178.254.23.111", "178.254.23.111"));           // Single node address



        */

        //vSeeds.push_back(CDNSSeedData("dnsseed1", "45.76.207.178"));
        //vSeeds.push_back(CDNSSeedData("dnsseed2", "207.148.121.234"));
        //vSeeds.push_back(CDNSSeedData("dnsseed3", "45.76.32.122"));
        //vSeeds.push_back(CDNSSeedData("dnsseed4", "45.32.147.218"));

        //vSeeds.push_back(CDNSSeedData("dnsseed5", "209.250.236.152"));
        //vSeeds.push_back(CDNSSeedData("dnsseed6", "207.148.83.18"));
        //vSeeds.push_back(CDNSSeedData("dnsseed7", "95.179.139.29"));
        //vSeeds.push_back(CDNSSeedData("dnsseed8", "45.32.92.37"));

        //vSeeds.push_back(CDNSSeedData("dnsseed9", "66.42.73.107"));
        //vSeeds.push_back(CDNSSeedData("dnsseed10", "104.238.136.127"));
        vSeeds.push_back(CDNSSeedData("dnsseed11", "217.61.18.194"));
        vSeeds.push_back(CDNSSeedData("dnsseed12", "95.216.154.7"));
        vSeeds.push_back(CDNSSeedData("dnsseed13", "107.191.63.1"));
        // vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 110);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 38);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 138);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x24)(0x33)(0x24).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x25)(0x32).convert_to_container<std::vector<unsigned char> >();
        //  BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x90)(0x32)(0x00)(0x83).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04dec88ebc6cd84506daf0aba91be0b7f2e0cb50e8d9185dee0bd08e1f345fa1410e04fee3f367ca3652802b2bb7fef72dd2c3f7553c0f29c914afce889cce5ba2";
        strSporkKeyOld = "04dec88ebc6cd84506daf0aba91be0b7f2e0cb50e8d9185dee0bd08e1f345fa1410e04fee3f367ca3652802b2bb7fef72dd2c3f7553c0f29c914afce889cce5ba2";
        strObfuscationPoolDummyAddress = "D87q2gC9j6nNrnzCsg4aY6bHMLsT9nUhEw";
        nStartMasternodePayments = 1403728576; //Wed, 25 Jun 2014 20:36:16 GMT

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 1; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 20; //The required confirmations for a mds to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0x4f;
        pchMessageStart[2] = 0x10;
        pchMessageStart[3] = 0xc2;
        vAlertPubKey = ParseHex("049715dd6f17f8fd9f54db8f175945e4511cae8d6b1906a1482d7e013dcb4f1e9b371eefb1b36c2da7be7472ee6aacb033a318610e780afc5a885122124e13b74a");
        nDefaultPort = 44441;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Midas: 1 day
        nTargetSpacing = 2 * 60;  // Midas: 1 minute
        nLastPOWBlock = 300;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 5000000 * COIN;
        nZerocoinStartHeight = 201576;
        nZerocoinStartTime = 1501776000;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 9902850; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 444020; //!> The block that zerocoin v2 becomes active
        nEnforceNewSporkKey = 1521604800; //!> Sporks signed after Wednesday, March 21, 2018 4:00:00 AM GMT must use the new spork key
        nRejectOldSporkKey = 1522454400; //!> Reject old spork key after Saturday, March 31, 2018 12:00:00 AM GMT
        nStakeInputMinimal = 20 * COIN;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1454124731;
        genesis.nNonce = 2402015;

        hashGenesisBlock = genesis.GetHash();
        //printf("testnet: hashGenesisBlock = %s\n",hashGenesisBlock.ToString().c_str());
        assert(hashGenesisBlock == uint256("0x2b206383510620f92ca0ee864b4419b8059c6574165dd070bd3a1924babe875c"));

        vFixedSeeds.clear();
        vSeeds.clear();
        /*vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "midas-testnet.seed.fuzzbawls.pw"));
        vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "midas-testnet.seed2.fuzzbawls.pw"));
        vSeeds.push_back(CDNSSeedData("s3v3nh4cks.ddns.net", "s3v3nh4cks.ddns.net"));
        vSeeds.push_back(CDNSSeedData("88.198.192.110", "88.198.192.110"));*/

        vSeeds.push_back(CDNSSeedData("dnsseed1", "45.76.207.178"));
        vSeeds.push_back(CDNSSeedData("dnsseed2", "207.148.121.234"));
        vSeeds.push_back(CDNSSeedData("dnsseed3", "45.76.32.122"));
        vSeeds.push_back(CDNSSeedData("dnsseed4", "45.32.147.218"));

        vSeeds.push_back(CDNSSeedData("dnsseed5", "209.250.236.152"));
        vSeeds.push_back(CDNSSeedData("dnsseed6", "207.148.83.18"));
        vSeeds.push_back(CDNSSeedData("dnsseed7", "95.179.139.29"));
        vSeeds.push_back(CDNSSeedData("dnsseed8", "45.32.92.37"));

        vSeeds.push_back(CDNSSeedData("dnsseed9", "66.42.73.107"));
        vSeeds.push_back(CDNSSeedData("dnsseed10", "104.238.136.127"));
        // vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 110);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 38);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 138);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x24)(0x33)(0x24).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x25)(0x32).convert_to_container<std::vector<unsigned char> >();
        //  BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x90)(0x32)(0x00)(0x83).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04a83b1f33a808ec8db19b13bbdadc50e910e69d39e4dfcdae61f2a907789b5a4c34b15988471e3b2188ac685c570a954fae20661449f8d983c415f7a141d4935d";
        strSporkKeyOld = "04a83b1f33a808ec8db19b13bbdadc50e910e69d39e4dfcdae61f2a907789b5a4c34b15988471e3b2188ac685c570a954fae20661449f8d983c415f7a141d4935d";
        strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
        nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa2;
        pchMessageStart[1] = 0x50;
        pchMessageStart[2] = 0x11;
        pchMessageStart[3] = 0xc3;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Midas: 1 day
        nTargetSpacing = 2 * 60;        // Midas: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1454124731;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 12345;
        nStakeInputMinimal = 20 * COIN;

        hashGenesisBlock = genesis.GetHash();
        //printf("regtest: hashGenesisBlock = %s\n",hashGenesisBlock.ToString().c_str());
        nDefaultPort = 44442;
        assert(hashGenesisBlock == uint256("0x77211018896d029616dcd7da8dd6096ef0ca0b620223f6efff25409814f953f0"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 44443;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
