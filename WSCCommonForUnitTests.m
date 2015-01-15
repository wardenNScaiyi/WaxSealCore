///:
/*****************************************************************************
 **                                                                         **
 **                               .======.                                  **
 **                               | INRI |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                      .========'      '========.                         **
 **                      |   _      xxxx      _   |                         **
 **                      |  /_;-.__ / _\  _.-;_\  |                         **
 **                      |     `-._`'`_/'`.-'     |                         **
 **                      '========.`\   /`========'                         **
 **                               | |  / |                                  **
 **                               |/-.(  |                                  **
 **                               |\_._\ |                                  **
 **                               | \ \`;|                                  **
 **                               |  > |/|                                  **
 **                               | / // |                                  **
 **                               | |//  |                                  **
 **                               | \(\  |                                  **
 **                               |  ``  |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                   \\    _  _\\| \//  |//_   _ \// _                     **
 **                  ^ `^`^ ^`` `^ ^` ``^^`  `^^` `^ `^                     **
 **                                                                         **
 **                       Copyright (c) 2015 Tong G.                        **
 **                          ALL RIGHTS RESERVED.                           **
 **                                                                         **
 ****************************************************************************/

#import "WSCCommon.h"
#import "WSCCommonForUnitTests.h"

#import "WSCKeychain.h"
#import "WSCKeychainPrivate.h"
#import "WSCKeychainManager.h"

#import "NSString+OMCString.h"
#import "NSURL+WSCKeychainURL.h"

NSString* _WSCTestPassword = @"waxsealcore";

WSCKeychainSelectivelyUnlockKeychainBlock _WSCSelectivelyUnlockKeychainsBasedOnPassword =
    ^( void )
        {
        CFArrayRef secSearchList = NULL;
        SecKeychainCopySearchList( &secSearchList );
        NSArray* searchList = [ [ WSCKeychainManager defaultManager ] keychainSearchList ];

        NSLog( @"SecKeychain SearchList Count: %lu", CFArrayGetCount( secSearchList ) );
        NSLog( @"Keychain SearchList Count: %lu", searchList.count );

        for ( WSCKeychain* _Keychain in searchList )
            {
            if ( [ _Keychain isEqualToKeychain: [ WSCKeychain login ] ] )
                {
                [ [ WSCKeychainManager defaultManager ] unlockKeychain: _Keychain withPassword: @"Dontbeabitch77!." error: nil ];
                continue;
                }

            if ( [ _Keychain.URL.path contains: @"withPrompt" ]
                    || [ _Keychain.URL.path contains: @"WithInteractionPrompt" ] )
                [ [ WSCKeychainManager defaultManager ] unlockKeychain: _Keychain withPassword: @"isgtforever" error: nil ];

            else if ( [ [ _Keychain.URL path ] contains: @"nonPrompt" ] )
                [ [ WSCKeychainManager defaultManager ] unlockKeychain: _Keychain withPassword: @"waxsealcore" error: nil ];
            }
        };

// Collection of random URLs,
// we will clear it and delete all keychains in it in __attribute__( ( constructor ) )s_commonTearDownForUnitTestModules() static function.
NSMutableSet* _WSCKeychainAutodeletePool;
__attribute__( ( constructor ) )
static void s_commonSetUpForUnitTestModules()
    {
    dispatch_once_t static onceToken;
    dispatch_once( &onceToken
                 , ( dispatch_block_t )^( void )
                    {
                    _WSCKeychainAutodeletePool = [ [ NSMutableSet set ] retain ];
                    } );
    }

// A valid public keychain which was shared between different test modules.
WSCKeychain* _WSCCommonValidKeychainForUnitTests;
__attribute__( ( constructor ) )
static void s_setUpCommonValidKeychain()
    {
    dispatch_once_t static onceToken;
    dispatch_once( &onceToken
                 , ( dispatch_block_t )^( void )
                    {
                    NSError* error = nil;

                    NSURL* URLForKeychain = [ [ NSURL URLForTemporaryDirectory ] URLByAppendingPathComponent: @"CommonValidKeychain.keychain" ];
                    _WSCCommonValidKeychainForUnitTests = [ [ [ WSCKeychain keychainWithURL: URLForKeychain
                                                                                   password: @"waxsealcore"
                                                                              initialAccess: nil
                                                                             becomesDefault: NO
                                                                                      error: &error ] retain ] autodelete ];
                    WSCPrintNSErrorForLog( error );
                    } );
    }

// An invalid public keychain (was deleted) which was shared between different test modules.
WSCKeychain* _WSCCommonInvalidKeychainForUnitTests;
__attribute__( ( constructor ) )
static void s_setUpCommonInvalidKeychain()
    {
    dispatch_once_t static onceToken;
    dispatch_once( &onceToken
                 , ( dispatch_block_t )^( void )
                    {
                    NSError* error = nil;

                    _WSCCommonInvalidKeychainForUnitTests = [ _WSCRandomKeychain() retain ];

                    // We need an invalid keychain, killed it in advance!
                    [ [ WSCKeychainManager defaultManager ] deleteKeychain: _WSCCommonInvalidKeychainForUnitTests
                                                                     error: &error ];
                    WSCPrintNSErrorForLog( error );
                    } );
    }

__attribute__( ( destructor ) )
static void s_commonTearDownForUnitTestModules()
    {
    for ( WSCKeychain* _Keychain in _WSCKeychainAutodeletePool )
        [ [ WSCKeychainManager defaultManager ] deleteKeychain: _Keychain error: nil ];
    }

NSURL* _WSCRandomURL()
    {
    srand( ( unsigned int )time( NULL ) );

    NSString* fuckString = [ NSString stringWithFormat: @"%lu", random() ];
    NSString* keychainNameWithHash = [ NSString stringWithFormat: @"%lx.keychain"
                                                                , fuckString.hash ];

    return [ [ NSURL URLForTemporaryDirectory ] URLByAppendingPathComponent: keychainNameWithHash ];
    }

WSCKeychain* _WSCRandomKeychain()
    {
    NSError* error = nil;
    NSURL* randomURL = _WSCRandomURL();
    WSCKeychain* randomKeychain = [ [ WSCKeychain p_keychainWithURL: randomURL
                                                           password: @"waxsealcore"
                                                     doesPromptUser: NO
                                                      initialAccess: nil
                                                     becomesDefault: NO
                                                              error: &error ] autodelete ];
    WSCPrintNSErrorForLog( error );
    return randomKeychain;
    }

NSURL* _WSCURLForTestCase( NSString* _TestCase, BOOL _DoesPrompt, BOOL _DeleteExits )
    {
    NSError* error = nil;
    NSString* keychainName = [ NSString stringWithFormat: @"WSC_%@_%@.keychain"
                                                        , _DoesPrompt ? @"withPrompt" : @"nonPrompt"
                                                        , _TestCase ];

    NSURL* newURL = [ [ NSURL URLForTemporaryDirectory ] URLByAppendingPathComponent: keychainName ];

    if ( _DeleteExits )
        {
        WSCKeychain* existKeychain = [ WSCKeychain keychainWithContentsOfURL: newURL error: &error ];

        if ( !error )
            {
            [ [ WSCKeychainManager defaultManager ] deleteKeychain: existKeychain error: &error ];
            WSCPrintNSErrorForLog( error );
            }
        else
            WSCPrintNSErrorForLog( error );
        }

    return newURL;
    }

#pragma mark Private Programmatic Interfaces for Ease of Unit Tests
@implementation WSCKeychain ( _WSCKeychainEaseOfUnitTests )

// Add the keychain into the auto delete pool
- ( instancetype ) autodelete
    {
    if ( self.isValid )
        [ _WSCKeychainAutodeletePool addObject: self ];

    return self;
    }

@end // WSCKeychain + WSCKeychainEaseOfUnitTests

//////////////////////////////////////////////////////////////////////////////

/*****************************************************************************
 **                                                                         **
 **                                                                         **
 **      █████▒█    ██  ▄████▄   ██ ▄█▀       ██████╗ ██╗   ██╗ ██████╗     **
 **    ▓██   ▒ ██  ▓██▒▒██▀ ▀█   ██▄█▒        ██╔══██╗██║   ██║██╔════╝     **
 **    ▒████ ░▓██  ▒██░▒▓█    ▄ ▓███▄░        ██████╔╝██║   ██║██║  ███╗    **
 **    ░▓█▒  ░▓▓█  ░██░▒▓▓▄ ▄██▒▓██ █▄        ██╔══██╗██║   ██║██║   ██║    **
 **    ░▒█░   ▒▒█████▓ ▒ ▓███▀ ░▒██▒ █▄       ██████╔╝╚██████╔╝╚██████╔╝    **
 **     ▒ ░   ░▒▓▒ ▒ ▒ ░ ░▒ ▒  ░▒ ▒▒ ▓▒       ╚═════╝  ╚═════╝  ╚═════╝     **
 **     ░     ░░▒░ ░ ░   ░  ▒   ░ ░▒ ▒░                                     **
 **     ░ ░    ░░░ ░ ░ ░        ░ ░░ ░                                      **
 **              ░     ░ ░      ░  ░                                        **
 **                    ░                                                    **
 **                                                                         **
 ****************************************************************************/