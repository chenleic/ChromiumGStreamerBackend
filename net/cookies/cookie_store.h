// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Brought to you by number 42.

#ifndef NET_COOKIES_COOKIE_STORE_H_
#define NET_COOKIES_COOKIE_STORE_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/callback_list.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "net/base/net_export.h"
#include "net/cookies/canonical_cookie.h"
#include "net/cookies/cookie_options.h"

class GURL;

namespace net {

class CookieMonster;

// An interface for storing and retrieving cookies. Implementations need to
// be thread safe as its methods can be accessed from IO as well as UI threads.
//
// All async functions may either invoke the callback asynchronously on the same
// thread, or they may be invoked immediately (prior to return of the
// asynchronous function).
class NET_EXPORT CookieStore : public base::RefCountedThreadSafe<CookieStore> {
 public:
  // Callback definitions.
  typedef base::Callback<void(const CookieList& cookies)> GetCookieListCallback;
  typedef base::Callback<void(const std::string& cookie)> GetCookiesCallback;
  typedef base::Callback<void(bool success)> SetCookiesCallback;
  typedef base::Callback<void(int num_deleted)> DeleteCallback;
  typedef base::Callback<void(const CanonicalCookie& cookie, bool removed)>
      CookieChangedCallback;
  typedef base::CallbackList<void(const CanonicalCookie& cookie, bool removed)>
      CookieChangedCallbackList;
  typedef CookieChangedCallbackList::Subscription CookieChangedSubscription;

  // Sets the cookies specified by |cookie_list| returned from |url|
  // with options |options| in effect.  Expects a cookie line, like
  // "a=1; domain=b.com".
  //
  // Fails either if the cookie is invalid or if this is a non-HTTPONLY cookie
  // and it would overwrite an existing HTTPONLY cookie.
  // Returns true if the cookie is successfully set.
  virtual void SetCookieWithOptionsAsync(
      const GURL& url,
      const std::string& cookie_line,
      const CookieOptions& options,
      const SetCookiesCallback& callback) = 0;

  // TODO(???): what if the total size of all the cookies >4k, can we have a
  // header that big or do we need multiple Cookie: headers?
  // Note: Some sites, such as Facebook, occasionally use Cookie headers >4k.
  //
  // Simple interface, gets a cookie string "a=b; c=d" for the given URL.
  // Gets all cookies that apply to |url| given |options|. Use options to
  // access httponly cookies.
  //
  // The returned cookies are ordered by longest path, then earliest
  // creation date.
  virtual void GetCookiesWithOptionsAsync(
      const GURL& url,
      const CookieOptions& options,
      const GetCookiesCallback& callback) = 0;

  // Invokes GetAllCookiesForURLWithOptions with options set to include HTTP
  // only cookies.
  virtual void GetAllCookiesForURLAsync(
      const GURL& url,
      const GetCookieListCallback& callback) = 0;

  // Returns all the cookies, for use in management UI, etc. This does not mark
  // the cookies as having been accessed. The returned cookies are ordered by
  // longest path, then by earliest creation date.
  virtual void GetAllCookiesAsync(const GetCookieListCallback& callback) = 0;

  // Deletes all cookies that might apply to |url| that have |cookie_name|.
  virtual void DeleteCookieAsync(const GURL& url,
                                 const std::string& cookie_name,
                                 const base::Closure& callback) = 0;

  // Deletes all of the cookies that have a creation_date greater than or equal
  // to |delete_begin| and less than |delete_end|
  // Returns the number of cookies that have been deleted.
  virtual void DeleteAllCreatedBetweenAsync(const base::Time& delete_begin,
                                            const base::Time& delete_end,
                                            const DeleteCallback& callback) = 0;

  // Deletes all of the cookies that match the host of the given URL
  // regardless of path and that have a creation_date greater than or
  // equal to |delete_begin| and less then |delete_end|. This includes
  // all http_only and secure cookies, but does not include any domain
  // cookies that may apply to this host.
  // Returns the number of cookies deleted.
  virtual void DeleteAllCreatedBetweenForHostAsync(
      const base::Time delete_begin,
      const base::Time delete_end,
      const GURL& url,
      const DeleteCallback& callback) = 0;

  virtual void DeleteSessionCookiesAsync(const DeleteCallback&) = 0;

  // Flush the backing store (if any) to disk and post the given callback when
  // done.
  // WARNING: THE CALLBACK WILL RUN ON A RANDOM THREAD. IT MUST BE THREAD SAFE.
  // It may be posted to the current thread, or it may run on the thread that
  // actually does the flushing. Your Task should generally post a notification
  // to the thread you actually want to be notified on.
  // TODO(mmenke):  Once this class is no longer thread-safe, this will always
  // be invoked on the CookieStore's thread, and this comment can be removed.
  // https://crbug.com/46185
  virtual void FlushStore(const base::Closure& callback) = 0;

  // Returns the underlying CookieMonster.
  virtual CookieMonster* GetCookieMonster() = 0;

  // Add a callback to be notified when the set of cookies named |name| that
  // would be sent for a request to |url| changes. The returned handle is
  // guaranteed not to hold a hard reference to the CookieStore object.
  //
  // |callback| will be called when a cookie is added or removed. |callback| is
  // passed the respective |cookie| which was added to or removed from the
  // cookies and a boolean indicating if the cookies was removed or not.
  //
  // Note that |callback| is called twice when a cookie is updated: once for
  // the removal of the existing cookie and once for the adding the new cookie.
  //
  // Note that this method consumes memory and CPU per (url, name) pair ever
  // registered that are still consumed even after all subscriptions for that
  // (url, name) pair are removed. If this method ever needs to support an
  // unbounded amount of such pairs, this contract needs to change and
  // implementors need to be improved to not behave this way.
  virtual scoped_ptr<CookieChangedSubscription> AddCallbackForCookie(
      const GURL& url,
      const std::string& name,
      const CookieChangedCallback& callback) = 0;

 protected:
  friend class base::RefCountedThreadSafe<CookieStore>;
  CookieStore();
  virtual ~CookieStore();
};

}  // namespace net

#endif  // NET_COOKIES_COOKIE_STORE_H_
