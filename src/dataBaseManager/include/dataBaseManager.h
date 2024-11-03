// ============================================================================
// ============================================================================
// dataBaseManager.h: 
// This is the "module" (component) that is responsible to manage and maintain
// all the values that were extracted from the different answers, i.e. - IPv4
// (A record type) or IPv6 (AAAA record type) or CNAME (canonical name).
// It does so by being a "proxy" to the hash table that holds all the values
// from all the DNS responses that were captured.
//
// High level implementation:
// The data base will be in the form of array in a fixed size as follows:
// - The index of each entry is the hash value of the string that represents
//   the domain name, for example, given domain name "google.com", the index
//   for it will be 33 (i.e., getHash("google.com") --> 33).
// - The value of each entry will be a linked list of all the IPv4/IPv6/CNAME
//   values that were captured for this specific domain name. For example:
//   Key: google.com, Values: 142.251.142.206 -> 8.8.8.8 -> NULL
// 
// ============================================================================
// ============================================================================
#pragma once

int dataBaseMgrInit();
int dataBaseMgrInsertItem();
void dataBaseMgrClean();