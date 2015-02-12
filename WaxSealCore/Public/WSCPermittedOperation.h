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

#import <Security/Security.h>

/** Defines constants that specify operations that can be done with that protected item, 
    such as decrypting, encrypting, sign or authenticating.
  */
typedef NS_ENUM( NSUInteger, WSCPermittedOperationTag )
    {
    /// Use for a CSP (smart card) login.
      WSCPermittedOperationTagLogin = 1

    /// Use for generating a key.
    , WSCPermittedOperationTagGenerateKey = 1 << 2

    /// Use for deleting this protected keychain item.
    , WSCPermittedOperationTagDelete = 1 << 3

    /// Use for encrypting data.
    , WSCPermittedOperationTagEncrypt = 1 << 4

    /// Use for decrypting data.
    , WSCPermittedOperationTagDecrypt = 1 << 5

    /// Use for exporting an encrypted key.
    /// This tag is checked on the key being exported;
    /// in addition, the `WSCPermittedOperationTagEncrypt` tag is checked for any key used in the encrypting operation.
    , WSCPermittedOperationTagExportEncryptedKey = 1 << 6

    /// Use for exporting an unencrypted key.
    , WSCPermittedOperationTagExportUnencryptedKey = 1 << 7

    /// Import an encrypted key.
    /// This tag is checked on the key being imported;
    /// in addition, the `WSCPermittedOperationTagDecrypt` tag is checked for any key used in the decrypting operation.
    , WSCPermittedOperationTagImportEncryptedKey = 1 << 8

    /// Use for importing an unencrypted key.
    , WSCPermittedOperationTagImportUnencryptedKey = 1 << 9

    /// Use for digitally signing data.
    , WSCPermittedOperationTagSign = 1 << 10

    /// Use for creating or verifying a message authentication code.
    , WSCPermittedOperationTagCreateOrVerifyMessageAuthCode = 1 << 11

    /// Use for deriving a new key from another key.
    , WSCPermittedOperationTagDerive = 1 << 12

    /// Use for changing the permitted operation itself.
    , WSCPermittedOperationTagChangePermittedOperationItself = 1 << 13

    /// For internal system use only.
    /// Use the `WSCPermittedOperationTagChangePermittedOperationItself` tag for changes to owner permitted operation entries.
    , WSCPermittedOperationTagChangeOwner = 1 << 14

    /// No restrictions. This permitted operation entry applies to all operations available to the caller.
    , WSCPermittedOperationTagAnyOperation = 1 << 15
    };

/** Masks that define when using a keychain or a protected keychain item should require a passphrase.
  */
typedef NS_ENUM( SecKeychainPromptSelector, WSCPermittedOperationPromptContext )
    {
    /// Indicates that a passphrase should be required for every access action.
      WSCPermittedOperationPromptContextRequirePassphraseEveryAccess = kSecKeychainPromptRequirePassphase

    /// Indicates that a passphrase should be required
    /// when an unsigned application attempts to use a keychain or a protected keychain item,
    /// overriding the system default.
    , WSCPermittedOperationPromptContextWhenUnsigned = kSecKeychainPromptUnsigned

    /// Indicates that a passphrase should be required
    /// when an unsigned application attempts to use a keychain or a protected keychain item.
    , WSCPermittedOperationPromptContextWhenUnsignedAct = kSecKeychainPromptUnsignedAct

    /// Indicates that a passphrase should be required
    /// when an application with an invalid signature attempts to use a the keychain or a protected keychain item,
    /// overriding the system default.
    , WSCPermittedOperationPromptContextInvalidSigned = kSecKeychainPromptInvalid

    /// Indicates that a passphrase should be required
    /// when an application with an invalid signature attempts to use a the keychain or a protected keychain item.
    , WSCPermittedOperationPromptContextInvalidSignedAct = kSecKeychainPromptInvalidAct
    };

/** The `WSCPermittedOperation` represents information about an permitted operation of a protected keychain item.

  In *WaxSealCore*, each protected keychain item (and the keychain itself) contains a list of **permitted operation**s
  in the form of a `WSCPermittedOperation` object.
  Each permitted operation has a list of one or more **permitted operation tag**s specifying operations
  that can be done with that protected item, such as decrypting, encrypting, sign or authenticating.
  In addition, each permitted operation has a list of **trusted application**s (represented by WSCTrustedApplication)
  that can perform the operations specified by the permitted operation tags without authorization from the user.
  
  When an OS X application attempts to access a keychain item for a particular purpose (such as to sign a document), 
  the system looks at each permitted operation entry in the permitted operaton list (represented by [permittedOperations](-[WSCProtectedKeychainItem permittedOperations]) ) 
  for that item to determine whether the application should be allowed access.
  If there is no corresponded permitted operation entry for that operation, then access is denied and it is up to the calling application
  to try something else or to notify the user. 
  If there are any permitted operation entry for the operation, then the system looks at the list of trusted applications
  of each permitted operation entry to see if the calling application is in the list. 
  If it is—or if the permitted operation specifies that all applications are allowed access—then access is permitted
  without confirmation from the user (as long as the keychain is unlocked). 
  If there is an permitted operation entry for the operation but the calling application is not in the list of 
  trusted applications, then the system prompts the user for the keychain password before permitting the application 
  to access the item.
  
  The *WaxSealCore* frameword provides API to create, delete, read, and modify permitted operation entries.
  There can be any number of permitted operation entries in the list of permitted operatons of an protected keychain item. 
  If two or more of the permitted operation entries are for the same operation, there is no way to predict the order in which they will be evaluated.
  
  `WSCPermittedOperation` is abstract of `SecACLRef`, `SecAccessRef` and their correspoding functions in *Keychain Services* API.
  Although the `WSCProtectedKeychainItem` and its subclasses and `WSCPermittedOperation` lets you create permitted operation entries 
  and add them to a protected keychain item, it is limited in the ways you can configure the authorizations and lists of trusted applications. 
  If you want to implement access controls that go beyond the complexity supported by *WaxSealCore*,
  you can use the bridge API: secACL property to retrieve the underlying `SecACLRef`, then work with the *Keychain Services* API
  or you can use the CSSM API to create a set of CSSM data structures and then call the `SecAccessCreateFromOwnerAndACL` function. 
  */
@interface WSCPermittedOperation : NSObject
    {
@protected
    SecACLRef _secACL;
    }

#pragma mark Attributes of Permitted Operations
/** @name Attributes of Permitted Operations */

/** The descriptor of the permitted operation represented by receiver.

  @discussion You can use this read-write property to set/get the name of the protected keychain item
              that appears in the dialog box when the user is prompted for permission to use the item. 
              Note that this descriptor is not necessarily the same as the one displayed for the item
              by the **Keychain Access** application.
  */
@property ( copy, readwrite ) NSString* descriptor;

#pragma mark Keychain Services Bridge
/** @name Keychain Services Bridge */

/** The reference of the `SecACL` opaque object, which wrapped by `WSCPermittedOperation` object. (read-only)
  
  @discussion If you are familiar with the underlying *Keychain Services* API,
              you can move freely back and forth between *WaxSealCore* framework and *Keychain Services* API with this property.
  */
@property ( unsafe_unretained, readonly ) SecACLRef secACL;

/** Creates and returns a `WSCPermittedOperation` object using the given reference to the instance of `SecACL` opaque type.

  This method creates a permitted operation object which represents information about an permitted operation of a protected keychain item 
  with the specified underlying `SecACLRef`.
  The permitted operation object can be inserted into the permittedOperations of a protected keychain item 
  which represented by WSCProtectedKeychainItem and its subclass.
  
  If you are familiar with the underlying *Keychain Services* API,
  you can move freely back and forth between *WaxSealCore* framework and *Keychain Services* API with this class method.

  @warning This method is just used for bridge between *WaxSealCore* framework and *Keychain Services* API.
  
  Instead of invoking this method, you should construct a `WSCPermittedOperation` object by invoking:

  + [- addPermittedOperationWithDescription:trustedApplications:forOperations:promptContext:error:](-[WSCProtectedKeychainItem addPermittedOperationWithDescription:trustedApplications:forOperations:promptContext:error:])

  @param _SecACLRef A reference to the instance of `SecACL` opaque type.
  
  @return A `WSCPermittedOperation` object initialized with the givent reference to the instance of `SecACL` opaque type.
          Return `nil` if *_SecACLRef* is `nil`.
  */
+ ( instancetype ) permittedOperationWithSecACLRef: ( SecACLRef )_SecACLRef;

@end // WSCPermittedOperation class

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