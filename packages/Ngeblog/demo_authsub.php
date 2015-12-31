<?php
/**
 * Simple demonstration on how to manage your Blogger with Ngeblog
 * using AuthSub authentication.
 *
 * @author Eris Ristemena <eristemena at ngoprekweb dot com>
 * @see http://www.ngoprekweb.com/tags/ngeblog
 * 
 */
 
  define(PATH_SEPARATOR,";");
  set_include_path(dirname(__FILE__) . '/Ngeblog'.PATH_SEPARATOR.dirname(__FILE__) . '/Zend_Gdata');
  require_once 'Ngeblog/AuthSub.php';
  
  session_start();
  
  if (!isset($_SESSION['authsub_token'])) {
	  if (isset($_GET['token'])) {
	    $session_token =  Ngeblog_AuthSub::getAuthSubSessionToken($_GET['token']);
    	$_SESSION['authsub_token'] = $session_token;
    } else {
      echo 'You must <a href="'.Ngeblog_AuthSub::getAuthSubTokenUri().'">login</a> first to use our service.';
      exit;
    }
  }
  else
	{
  	if ($_GET['cmd']=='logout') {
  	  Ngeblog_AuthSub::AuthSubRevokeToken($_SESSION['authsub_token']);
      unset($_SESSION['authsub_token']);
      header('Location: '.$_SERVER['PHP_SELF']);
      exit;
    }
  }
  
  try {
    $myblog = Ngeblog_AuthSub::Connect($_SESSION['authsub_token']);
    
    if ( $_POST['act']=='newPost' ) // add new post
    {
      $myblog->newPost(stripslashes($_POST['title']),stripslashes($_POST['content']),$_POST['blogid']);
      
      header('Location: '.$_SERVER['PHP_SELF'].'?blogid='.$_POST['blogid']);
      exit;
    }
    elseif ( $_POST['act']=='editPost' ) // edit a post
    {
      $myblog->editPost($_POST['entryid'],stripslashes($_POST['title']),stripslashes($_POST['content']),$_POST['blogid']);
      
      header('Location: '.$_SERVER['PHP_SELF'].'?blogid='.$_POST['blogid']);
      exit;
    }
    elseif ( $_GET['act']=='deletePost' && isset($_GET['blogid']) && isset($_GET['entryid']) )
    {
      $myblog->deletePost($_GET['entryid'],$_GET['blogid']);
      
      header('Location: '.$_SERVER['PHP_SELF'].'?blogid='.$_GET['blogid']);
      exit;
    }
    
    // top panel
    echo '<table width="100%">';
    echo '<tr><td align="right">';
    echo '<a href="'.$_SERVER['PHP_SELF'].'">dashboard</a> ';
    if ( isset($_GET['blogid']) && isset($_GET['entryid']) ) {
      echo '<a href="'.$_SERVER['PHP_SELF'].'?blogid='.$_GET['blogid'].'">list post</a> ';
    }
    echo '<a href="'.$_SERVER['PHP_SELF'].'?cmd=logout">logout</a> ';
    echo '</td></tr>';
    echo '</table>';
    echo '<hr />';
    
    if ( isset($_GET['blogid']) && isset($_GET['entryid']) ) 
    {
      // get an entry
      $post = $myblog->getPost($_GET['entryid'],$_GET['blogid']);
      
      if ( $_GET['cmd']=='editPost' )
      {
        // show update form
        echo '<form action="'.$_SERVER['PHP_SELF'].'" method="post">';
        echo 'Title:<br /><input type="text" name="title" size="45" value="'.$post['content'].'" /><br />';
        echo 'Content:<br /><textarea name="content" cols="45" rows="20">'.$post['content'].'</textarea><br />';
        echo '<input type="hidden" name="blogid" value="'.$_GET['blogid'].'">';
        echo '<input type="hidden" name="entryid" value="'.$_GET['entryid'].'">';
        echo '<input type="hidden" name="act" value="editPost">';
        echo '<input type="submit" value="Update" />';
        echo '</form>';
      }
      else
      {
        // view post
        echo 'Title : <b>'.$post['title'].'</b><br />';
        echo 'Last Updated : <b>'.$post['updated'].'</b><br />';
        echo 'Content : <br />'.$post['content'];
      }
    }
    elseif ( isset($_GET['blogid']) )
    {
      if ( $_GET['cmd']=='newPost' ) 
      {
        // show new post form
        echo '<form action="'.$_SERVER['PHP_SELF'].'" method="post">';
        echo 'Title:<br /><input type="text" name="title" size="45" value="'.$post['content'].'" /><br />';
        echo 'Content:<br /><textarea name="content" cols="45" rows="20">'.$post['content'].'</textarea><br />';
        echo '<input type="hidden" name="blogid" value="'.$_GET['blogid'].'">';
        echo '<input type="hidden" name="act" value="newPost">';
        echo '<input type="submit" value="Submit" />';
        echo '</form>';
      }
      else
      {
        // show list of entries
        $_posts = $myblog->getPosts(1,10,$_GET['blogid']);
        echo '<table cellpadding="4" border="1">';
        echo '<tr><td>Entry ID</td><td>Title</td><td>Last Updated</td><td>&nbsp;</td></tr>';
        foreach( $_posts as $post )
        {
          echo '<tr>'.
              '<td>'.$post['entryid'].'</td>'.
              '<td>'.$post['title'].'</td>'.
              '<td>'.$post['updated'].'</td>'.
              '<td>'.
                '<a href="'.$_SERVER['PHP_SELF'].'?blogid='.$_GET['blogid'].'&entryid='.$post['entryid'].'">view</a> '.
                '<a href="'.$_SERVER['PHP_SELF'].'?cmd=editPost&blogid='.$_GET['blogid'].'&entryid='.$post['entryid'].'">edit</a> '.
                '<a href="'.$_SERVER['PHP_SELF'].'?act=deletePost&blogid='.$_GET['blogid'].'&entryid='.$post['entryid'].'">delete</a> '.
              '</td>'.
             '</tr>';
        }
        echo '<tr><td colspan="4" align="right"><a href="'.$_SERVER['PHP_SELF'].'?cmd=newPost&blogid='.$_GET['blogid'].'">new post</a></td></tr>';
        echo '</table>';
      }
    }
    else
    {
      // Dashboard
      $_bloginfo = $myblog->getBlogInfo();
      echo '<table cellpadding="4" border="1">';
      echo '<tr><td>Blog ID</td><td>Title</td><td>URL</td><td>&nbsp;</td></tr>';
      foreach ( $_bloginfo as $bloginfo ) 
      {
        // show list of blog for your account
        echo '<tr>'.
              '<td>'.$bloginfo['blogid'].'</td>'.
              '<td>'.$bloginfo['title'].'</td>'.
              '<td>'.$bloginfo['url'].'</td>'.
              '<td>'.
                '<a href="'.$_SERVER['PHP_SELF'].'?blogid='.$bloginfo['blogid'].'">browse</a> '.
              '</td>'.
             '</tr>';
      }
      echo '</table>';
    }
  } catch ( Exception $e )  {
    echo $e->getMessage();
    exit;
  }
  
?>