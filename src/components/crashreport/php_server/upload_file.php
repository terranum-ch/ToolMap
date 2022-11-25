<?php
print_r($_POST);    // POST fields
print_r($_FILES);    // file info and metadata

if (isset($_POST['filename'])) {
    // file upload
    if (is_uploaded_file($_FILES['filecontents']['tmp_name'])) {
        $path_parts = pathinfo($_SERVER["SCRIPT_FILENAME"]);
        $file_name = basename($_FILES['filecontents']['name']);
        $ext = pathinfo($file_name, PATHINFO_EXTENSION);

        /*if($ext != "zip"){
            echo "file extension not supported!";
            return;
        }*/

        $upload_file = $path_parts['dirname'] . '/' . $file_name;
        move_uploaded_file($_FILES['filecontents']['tmp_name'], $upload_file);

        $upload_success = false;
        $upload_status = "Failed!";
        switch ($_FILES['filecontents']['error']) {
            case UPLOAD_ERR_OK:
                $upload_success = true;
                $upload_status = "OK";
                chmod($upload_file, 0644);    // chown not possible with www-data (Apache)
                break;
            case UPLOAD_ERR_FORM_SIZE:
            case UPLOAD_ERR_INI_SIZE:
                echo "File Size is too big";
                $upload_status = "File too Big!";
                break;
            case UPLOAD_ERR_PARTIAL:
                echo "File partially uploaded! connection problem";
                $upload_status = "Partial upload, connection problem!";
                break;
            default:
                echo "File not uploaded!";
        }

        // send mail
        if (!sendMail("toolmap@terranum.ch", $_POST['softname'], $_POST['filename'], $upload_status)) {
            echo "No Mail sent!";
        }


    } else {
        echo "'filecontents' is not defined! \r\n";
    }
} else {
    echo 'Nothing to upload';
}


function sendMail($email, $software, $filename, $status)
{
    $message = "<html><body><p>New crash report from '$software'</p><ul>";
    $message .= "<li><strong>Name:</strong> $filename </li>";
    $message .= "<li><strong>Status:</strong> $status </li>";
    $message .= "</ul></body></html>";
    $subject = "[CRASH] - $software";
    $header = 'MIME-Version: 1.0' . "\r\n";
    $header .= 'Content-type: text/html; charset=UTF-8' . "\r\n";

    if (mail($email, $subject, $message, $header)) {
        return TRUE;
    }
    return FALSE;
}

?>