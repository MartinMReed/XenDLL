/*
* for a given element, make it visible depending on hide
* true = not visible
*/
function hideLayer(id, hide) {
   var elem = document.getElementById(id);
   if (hide) {
      elem.style.display = 'none';
   } else {
      elem.style.display = 'block';
   }
}

/*
* display a label over an input element
*/
function overLabel(input, label, hide) {
   var elem = document.getElementById(input);
   if (hide) {
      hideLayer(label, true);
   } else if (elem.value == '') {
      hideLayer(label, false);
   }
}

/*
* append an option to a given select list,
* by taking the value from a given drop down list
*/
function appendOptionFirst(idTo, idFrom)
{
   var elemTo = document.getElementById(idTo);
   var elemFrom = document.getElementById(idFrom);
   
   for (var i = elemTo.length; i > 0; i--) {
      elemTo.options[i] = new Option(elemTo.options[i-1].text, elemTo.options[i-1].value);
   }
   
   var optFrom = elemFrom.options[elemFrom.selectedIndex];
   elemTo.options[0] = new Option(optFrom.text, optFrom.value);
}

/*
* remove all selected options in a given select list
*/
function removeOptionSelected(id)
{
   var elem = document.getElementById(id);
   for (var i = elem.length - 1; i >= 0; i--) {
      if (elem.options[i].selected) {
         elem.remove(i);
      }
   }
}

/*
* select all options in a given select list
*/
function selectAllOptions(id)
{
   var elem = document.getElementById(id);
   for (var i = 0; i < elem.length; i++) {
      elem.options[i].selected = true;
   }
}

/*
* remove all options in a given select list
*/
function removeAllOptions(id)
{
   var elem = document.getElementById(id);
   for (var i = elem.length - 1; i >= 0 ; i--) {
      elem.options[i] = null;
   }
}

/*
* move selected options up in a given select list
*/
function moveOptionsUp(id)
{
   var elem = document.getElementById(id);
   for (var i = 1; i < elem.length; i++) {
      if (elem.options[i].selected == true) {
         elem.options[i].selected = false;
         var opt = new Array(new Option(elem.options[i-1].text, elem.options[i-1].value),
                        new Option(elem.options[i].text, elem.options[i].value));
         elem.options[i-1] = opt[1];
         elem.options[i] = opt[0];
      }
   }
}

/*
* move selected options down in a given select list
*/
function moveOptionsDown(id)
{
   var elem = document.getElementById(id);
   for (var i = elem.length - 2; i >= 0; i--) {
      if (elem.options[i].selected == true) {
         elem.options[i].selected = false;
         var opt = new Array(new Option(elem.options[i+1].text, elem.options[i+1].value),
                        new Option(elem.options[i].text, elem.options[i].value));
         elem.options[i+1] = opt[1];
         elem.options[i] = opt[0];
      }
   }
}

/*
* set the innerText of a <span> element
*/
function setSpanText(id, text)
{
   var elem = document.getElementById(id);
   elem.innerHTML = text;
}

/*
* add a hidden input element to a div
*/
function addHiddenElement(rootID, name, value) {
	var root = document.getElementById(rootID);
	var element = document.createElement('input');
	element.setAttribute('type', 'hidden');
	element.setAttribute('id', name);
	element.setAttribute('name', name);
	element.setAttribute('value', value);
	root.appendChild(element);
}

/*
* round the decimal places of a given number (value)
*/
function roundDouble(value, length)
{
   if (value > 8191 && value < 10485) {
      value = value-5000;
      var newnumber = Math.round(value*Math.pow(10,length))/Math.pow(10,length);
      newnumber = newnumber+5000;
   } else {
      var newnumber = Math.round(value*Math.pow(10,length))/Math.pow(10,length);
   }
   
   return newnumber;
}

/*
* format a number to look like currency. changes 2.1 to 2.10
*/
function formatCurrency(text)
{
   var part = text.split(".");
   if (part[0] == null || part[0].length == 0) part[0] = "0";
   if (part[1] == null || part[1].length == 0) part[1] = "00";
   if (part[1].length == 1) part[1] += "0";
   return part[0]+"."+part[1];
}

/*
* given a text field with a number, limit the input.
*/
function overageControl(id, limit)
{
   var elem = document.getElementById(id);
   var num = elem.value*1.00;
   if (num > limit) {
      elem.value = formatCurrency('' + limit);
   }
}

/*
* HOW TO USE FORMAT CHECK
*
* error span:
*       <span id="error-something" class="error" ></span>
*
* after the button to be disabled has been made, call:
*       <script type="text/javascript" language="javascript">
*           setFormatCheck(type, input id, error span id, button id);
*       </script>
*
* type will choose which regular expression to use (DOUBLE, CURRENCY, etc)
* inputs will be put into sets using the button id as a key
*
* for using reset, in the form tag call:
*       onreset="resetFormatList(button id, null);" // reset full set
* or
*       onreset="resetFormatList(button id, type);" // reset a type in a set
*/

/*
* a list of element ids to use for a format check
*/
var formatCheckList = new Array();

/*
* add a text input (id) to the format check list
* type will choose which regular expression to use (DOUBLE, CURRENCY, etc)
* inputs will be put into sets using the button id as a key
* idButton is the id of the button to disable when the check fails
*
* adding the same id + idButton combination more then once, will replace the old with the new
*/
function setFormatCheck(type, id, idSpan, idButton)
{
   var found = false;
   for (var i = 0; !found && i < formatCheckList.length; i++) {
      if (formatCheckList[i][0] == idButton) {
         var duplicate = false;
         var j;
         for (j = 1; !duplicate && j < formatCheckList[i].length; j++) {
            duplicate = (formatCheckList[i][j][1] == id);
         }
         
         if (duplicate) {
            formatCheckList[i][j-1] = new Array(type, id, idSpan);
         } else {
            formatCheckList[i] = formatCheckList[i].concat(new Array(new Array(type, id, idSpan)));
         }
         found = true;
      }
   }
   
   if (found == false) {
      if (formatCheckList.length == 0) {
         formatCheckList = new Array(new Array(idButton, new Array(type, id, idSpan)));
      } else {
         formatCheckList = formatCheckList.concat(new Array(new Array(idButton, new Array(type, id, idSpan))));
      }
   }
   
   var func = function () { formatCheck(idButton); };
   var elem = document.getElementById(id);
   elem.addEventListener('keydown', func, false);
   elem.addEventListener('keyup', func, false);
   elem.addEventListener('keypress', func, false);
   elem.addEventListener('blur', func, false);
}

/*
* check the format of given text inputs
*/
function formatCheck(idButton)
{
   var spanTextArr = new Array();
   var disable = false;
   var foundSet = false;
   for (var i = 0; !foundSet && i < formatCheckList.length; i++) {
      if (formatCheckList[i][0] == idButton) {
         for (var j = 1; j < formatCheckList[i].length; j++) {

            var elem = document.getElementById(formatCheckList[i][j][1]);
            var message = '';
            
            var regex;
            switch (formatCheckList[i][j][0]) {
               case "DOUBLE":
                  regex = new RegExp("^[0-9]+\\.?[0-9]*$|^[0-9]*\\.[0-9]+$");
                  break;
               case "CURRENCY":
                  regex = new RegExp("^[0-9]+\\.?[0-9]{0,2}$|^[0-9]*\\.[0-9]{1,2}$");
                  break;
               case "INTEGER":
                  regex = new RegExp("^0$|^[1-9]+[0-9]*$");
                  break;
               default:
                  regex = new RegExp(".*");
            }
            
            if (elem.value == regex.exec(elem.value) == false) {
               message = 'Invalid Format';
               disable = true;
            }
            
            var foundSpanText = false;
            for (var k = 0; !foundSpanText && k < spanTextArr.length; k++) {
               if (spanTextArr[k][0] == formatCheckList[i][j][2]) {
                  if (spanTextArr[k][1] == '') {
                     spanTextArr[k][1] = message;
                  }
                  foundSpanText = true;
               }
            }
            
            if (foundSpanText == false) {
               if (foundSpanText.length == 0) {
                  spanTextArr = new Array(new Array(formatCheckList[i][j][2], message));
               } else {
                  spanTextArr = spanTextArr.concat(new Array(new Array(formatCheckList[i][j][2], message)));
               }
            }
         }
         
         for (var j = 0; j < spanTextArr.length; j++) {
            setSpanText(spanTextArr[j][0], spanTextArr[j][1]);
         }
         
         foundSet = true;
      }
   }
   
   var elem = document.getElementById(idButton);
   elem.disabled = disable;
}

/*
* reset all items in formatCheckList with the given button id as a key
* to reset the full set, make type = null
*/
function resetFormatList(idButton, type)
{
   var reset = false;
   for (var i = 0; !reset && i < formatCheckList.length; i++) {
      if (formatCheckList[i][0] == idButton) {
         for (var j = 1; j < formatCheckList[i].length; j++) {
            if (type == null || formatCheckList[i][j][0] == type) {
               var elem = document.getElementById(formatCheckList[i][j][1]);
               elem.value = "";
               setSpanText(formatCheckList[i][j][2], '');
            }
         }
         
         var elem = document.getElementById(idButton);
         elem.disabled = false;
         
         reset = true;
      }
   }
}