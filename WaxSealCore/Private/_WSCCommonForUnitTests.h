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

#if DEBUG
#import "WSCKeychain.h"
#import "WSCKeychainItem.h"

@class NSMutableSet;

typedef void ( ^WSCKeychainSelectivelyUnlockKeychainBlock )( void );

WSCKeychainSelectivelyUnlockKeychainBlock extern _WSCSelectivelyUnlockKeychainsBasedOnPassphrase;

WSCKeychain  extern* _WSCCommonValidKeychainForUnitTests;
WSCKeychain  extern* _WSCCommonInvalidKeychainForUnitTests;
NSString     extern* _WSCTestPassphrase;
NSURL*          _WSCRandomURL();
WSCKeychain*    _WSCRandomKeychain();
NSURL* _WSCURLForTestCase( SEL _TestCase, NSString* _TestCaseDesc, BOOL _DoesPrompt, BOOL _DeleteExits );

WSCPassphraseItem* _WSC_www_waxsealcore_org_InternetKeychainItem( NSError** _Error );
WSCPassphraseItem* _WSC_WaxSealCoreTests_ApplicationKeychainItem( NSError** _Error );
void _WSCPrintAccess( SecAccessRef _Access );

#pragma mark Private Programmatic Interfaces for Ease of Unit Tests
@interface WSCKeychain ( _WSCKeychainEaseOfUnitTests )

// Add the keychain into the auto delete pool
- ( instancetype ) autodelete;

@end // WSCKeychain + WSCKeychainEaseOfUnitTests

@interface WSCKeychainItem ( _WSCKeychainItemEaseOfUnitTests )

// Add the keychain item into the auto delete pool
- ( instancetype ) autodelete;

@end // WSCKeychain + WSCKeychainEaseOfUnitTests

#endif

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